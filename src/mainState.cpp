#include "mainState.hpp"

cMainState::cMainState (void): cGameState(eStateAction::NONE,
		eStateAction::REMOVE_STATE), world_(nullptr), ship_(nullptr),
		walls_(nullptr), shield1_(nullptr), shield2_(nullptr),
		shield3_(nullptr), shield4_(nullptr), leftSensor_(nullptr),
		rightSensor_(nullptr), groundSensor_(nullptr),
		rng_(time(NULL)) {
	setSettings(15,5);
	cntrlKb_.addCommand(eKbAction::M_LEFT,SDLK_a);
	cntrlKb_.addCommand(eKbAction::M_RIGHT,SDLK_d);
	cntrlKb_.addCommand(eKbAction::SHOOT,SDLK_s);
	cntrlKb_.addCommand(eKbAction::TOGGLE_DEBUG,SDLK_F3,false);
	cntrlKb_.addCommand(eKbAction::QUIT,SDLK_ESCAPE);

//	eBroadphaseType broadphaseType = eBroadphaseType::GENERAL;
	eBroadphaseType broadphaseType = eBroadphaseType::GRID;
	switch (broadphaseType)
	{
		case eBroadphaseType::GENERAL:
			{
				world_ = new cCollWorld(eBroadphaseType::GENERAL);
				break;
			}
		case eBroadphaseType::GRID:
			{
				cVector2 worldDim(640,480);
				world_ = new cCollWorld(eBroadphaseType::GRID,
						static_cast<void*>(&worldDim));
				cGridBroadphase* broadphase = static_cast<cGridBroadphase*>(
						world_->getBroadphase());
				broadphase->setCellSize(10,10);
				break;
			}
		default: break;
	}
	drawer_ = new cCollDebugDrawer;
	world_->setDebugDraw(drawer_);

	// Create the ship entity
	cPosComp shipPos(0,0,0);

	cCollAabb shipShape(10,10);

	cEntityNode shipNode(0,shipPos,cCollComp(shipShape));

	ship_ = world_->createEntity(0,cPosComp(320,330,0),shipNode,
			eEntityType::DYNAMIC,SHIP_MASK);
	ship_->setUsrPtr(static_cast<void*>(&shipInfo_));
	world_->addCollMask(SHIP_MASK,SHIP_COLL_MASK);
	
	// Create the walls
	int wallHW = 200, wallHH = 100;
	cPosComp wallPos1(-wallHW,0,0),
			 wallPos2(wallHW,0,0),
			 wallPos3(0,-wallHH,0),
			 wallPos4(0,wallHH,0);

	cCollAabb vertWall(5,wallHH),
			  horzWall(wallHW,5);

	std::vector<cEntityNode> walls;
	walls.push_back(cEntityNode(0,wallPos1,cCollComp(vertWall)));
	walls.push_back(cEntityNode(1,wallPos2,cCollComp(vertWall)));
	walls.push_back(cEntityNode(2,wallPos3,cCollComp(horzWall)));
	walls.push_back(cEntityNode(3,wallPos4,cCollComp(horzWall)));

	walls_ = world_->createEntity(1,cPosComp(320,240,0),walls,
			eEntityType::STATIC,WALL_MASK);
	world_->addCollMask(WALL_MASK,WALL_COLL_MASK);

	// Create the shields
	cCollAabb shieldShape(2.5,2.5);

	cPosComp posUL(-2.5,-2.5,0),
			 posUR(2.5,-2.5,0),
			 posLL(-2.5,2.5,0),
			 posLR(2.5,2.5,0);

	std::vector<cEntityNode> shieldNodes;
	shieldNodes.push_back(cEntityNode(0,posUL,cCollComp(shieldShape)));
	shieldNodes.push_back(cEntityNode(1,posUR,cCollComp(shieldShape)));
	shieldNodes.push_back(cEntityNode(2,posLL,cCollComp(shieldShape)));
	shieldNodes.push_back(cEntityNode(3,posLR,cCollComp(shieldShape)));

	cPosComp posShield1(282.5,300,0),
			 posShield2(307.5,300,0),
			 posShield3(332.5,300,0),
			 posShield4(357.5,300,0);

	shield1_ = world_->createEntity(2,posShield1,shieldNodes,
			eEntityType::STATIC,SHIELD_MASK);
	shield1_->setUsrPtr(static_cast<void*>(&shield1Hp_));
	shield2_ = world_->createEntity(3,posShield2,shieldNodes,
			eEntityType::STATIC,SHIELD_MASK);
	shield2_->setUsrPtr(static_cast<void*>(&shield2Hp_));
	shield3_ = world_->createEntity(4,posShield3,shieldNodes,
			eEntityType::STATIC,SHIELD_MASK);
	shield3_->setUsrPtr(static_cast<void*>(&shield3Hp_));
	shield4_ = world_->createEntity(5,posShield4,shieldNodes,
			eEntityType::STATIC,SHIELD_MASK);
	shield4_->setUsrPtr(static_cast<void*>(&shield4Hp_));
	world_->addCollMask(SHIELD_MASK,SHIELD_COLL_MASK);

	// Create the alien array (11x5) and sensors
	double hw = 4.0,
		   hh = 4.0,
		   space = 10.0,
		   dx = 2*hw+space,
		   dy = 2*hh+space,
		   xCentre = 320.0,
		   yCentre = 240.0;
	
	cCollAabb alienShape(hw,hh);
	cCollComp alienCollComp(alienShape);
	cEntityNode alienNode(0,cPosComp(0,0,0),alienCollComp);

	cCollPoint sensorPoint;
	cCollComp sensorComp(sensorPoint);
	cEntityNode sensorNode(0,cPosComp(0,0,0),sensorComp);

	std::vector<double> xPos(11,0.0),
						yPos(5,0.0);
	xPos[0] = xCentre+(-5.0*dx);
	xPos[1] = xCentre+(-4.0*dx);
	xPos[2] = xCentre+(-3.0*dx);
	xPos[3] = xCentre+(-2.0*dx);
	xPos[4] = xCentre+(-1.0*dx);
	xPos[5] = xCentre;
	xPos[6] = xCentre+(1.0*dx);
	xPos[7] = xCentre+(2.0*dx);
	xPos[8] = xCentre+(3.0*dx);
	xPos[9] = xCentre+(4.0*dx);
	xPos[10] = xCentre+(5.0*dx);

	yPos[0] = yCentre+(-2.0*dy);
	yPos[1] = yCentre+(-1.0*dy);
	yPos[2] = yCentre;
	yPos[3] = yCentre+(1.0*dy);
	yPos[4] = yCentre+(2.0*dy);

	cPosComp rSensPos(xPos[10]+hw,yPos[0],0.0),
			 lSensPos(xPos[0]-hw,yPos[0],0.0);
	leftSensor_ = world_->createEntity(6,lSensPos,sensorNode,
			eEntityType::DYNAMIC,ALIEN_SENSOR_MASK);
	leftSensor_->setUsrPtr(static_cast<void*>(&alienInfo_));
	rightSensor_ = world_->createEntity(7,rSensPos,sensorNode,
			eEntityType::DYNAMIC,ALIEN_SENSOR_MASK);
	rightSensor_->setUsrPtr(static_cast<void*>(&alienInfo_));
	world_->addCollMask(ALIEN_SENSOR_MASK,ALIEN_SENSOR_COLL_MASK);

	int alienId = 100;

	for (int i = 0; i < 11; ++i) {
		for (int j = 0; j < 5; ++j) {
			cPosComp alienPos(xPos[i],yPos[j],0.0);
			aliens_.push_back(world_->createEntity(alienId,alienPos,alienNode,
					eEntityType::DYNAMIC,ALIEN_MASK));
			++alienId;
		}
	}
	for (auto& itr : aliens_) {
		itr->setUsrPtr(static_cast<void*>(&alienInfo_));
	}
	world_->addCollMask(ALIEN_MASK,ALIEN_COLL_MASK);

	// Create the ground sensor
	cCollAabb gSensShape(200,50);
	cCollComp gSensCollComp(gSensShape);
	cEntityNode gSensNode(0,cPosComp(0.0,0.0,0.0),gSensCollComp);
	groundSensor_ = world_->createEntity(8,cPosComp(320,400,0),gSensNode,
			eEntityType::STATIC,GROUND_SENSOR_MASK);
	world_->addCollMask(GROUND_SENSOR_MASK,GROUND_SENSOR_COLL_MASK);

	// Create the bullets
	cCollAabb bulletShape(1.0,1.0);

	cPosComp bulletPos(0.0,0.0,0);
	cPosComp bulletPos1(300,100,0);

	cCollComp bulletCollComp(bulletShape);
	
	cEntityNode bulletNode(0,bulletPos,bulletCollComp);

	bulletArray_[0] = world_->createEntity(9,bulletPos1,bulletNode,
			eEntityType::DYNAMIC,SHIP_BULLET_MASK);
	bulletArray_[0]->setNodeActivity(0,false);
	for (int i = 1; i < 12; ++i)
	{
		bulletArray_[i] = world_->createEntity(9+i,bulletPos1,bulletNode,
				eEntityType::DYNAMIC,ALIEN_BULLET_MASK);
		bulletArray_[i]->setNodeActivity(0,false);
	}
	world_->addCollMask(SHIP_BULLET_MASK,SHIP_BULLET_COLL_MASK);
	world_->addCollMask(ALIEN_BULLET_MASK,ALIEN_BULLET_COLL_MASK);

	// Final parameters, move these to the top after final values are found
	playerBulletVel_ = 3.0;
	alienBulletVel_ = 0.5;
	shipVel_ = 1.0;

	showDebug_ = false;
	lSensColIndex_ = 0;
	rSensColIndex_ = 10;
}

cMainState::~cMainState (void){
	delete drawer_;
	drawer_ = nullptr;
	delete world_;
	world_ = nullptr;
}

void cMainState::handleState (SDL_Event& event)
{
	switch (event.type) {
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			cntrlKb_.checkCommand(event.key,&kbActions_);
			break;
		case SDL_MOUSEMOTION:
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			break;
		default:
			break;
	}
}

int cMainState::updateState (double tickRate, void** interStateInfo) {
	cntrlKb_.removeRepeatCommands(&kbActions_);
	// Move the player ship, shoot or quit
	double playerDir = 0;
	if (kbActions_.size() > 0) {
		for (auto& kbCommand : kbActions_) {
			switch (kbCommand) {
				case eKbAction::M_LEFT:
					playerDir -= 1.0;;
					break;
				case eKbAction::M_RIGHT:
					playerDir += 1.0;
					break;
				case eKbAction::SHOOT:
					bulletArray_[0]->setNodeActivity(0,true);
					bulletArray_[0]->setPos(ship_->getPos());
					break;
				case eKbAction::TOGGLE_DEBUG:
					showDebug_ = !showDebug_;
					toggleRenderSettings(RENDER_FPS);
					toggleRenderSettings(RENDER_TPS);
					break;
				case eKbAction::QUIT:
					return eStateAction::REMOVE_STATE;
					break;
				default:
					break;
			}
		}
	}
	// Check for end game
	if (alienInfo_.endState == eEndState::GAME_OVER) {
		return eStateAction::END_STATE;
	}
	if (alienInfo_.totalAliens == 0) {
		return eStateAction::MAIN_STATE;
	}
	if (shipInfo_.numShips <= 0) {
		return eStateAction::END_STATE;
	}

	double baseTickSpeed = 60.0;
	double tickSpeedMult = baseTickSpeed/tickRate;

	// Move the ship
	double shipDisp = shipVel_*playerDir*tickSpeedMult;
	ship_->translate(shipDisp,0);

	// Move the aliens and sensors
	double alienVelX = alienInfo_.alienBaseSpeed * alienInfo_.alienSpeedMult *
		alienInfo_.alienDir*tickSpeedMult,
		   alienVelY = alienInfo_.alienVertOffset * alienInfo_.alienVertDir;
	for (auto& itr : aliens_) {
		itr->translate(alienVelX,alienVelY);
	}
	leftSensor_->translate(alienVelX,alienVelY);
	rightSensor_->translate(alienVelX,alienVelY);
	if (alienVelY != 0.0)
		alienInfo_.alienVertDir = 0.0;

	// Translate the sensors in case the aliens in a col have been 
	// destroyed
	int leftCol = lSensColIndex_,
		rightCol = rSensColIndex_;
	for (; leftCol < 11; ++leftCol) {
		if (alienInfo_.numAlienCol[leftCol] != 0)
			break;
	}
	for (; rightCol > 0; --rightCol) {
		if (alienInfo_.numAlienCol[rightCol] != 0)
			break;
	}
	int alienWidth = 18; // This is the width of the alien and spacing
	if (leftCol != lSensColIndex_) {
		leftSensor_->translate((leftCol-lSensColIndex_)*alienWidth,0);
		lSensColIndex_ = leftCol;
	}
	if (rightCol != rSensColIndex_) {
		rightSensor_->translate((rightCol-rSensColIndex_)*alienWidth,0);
		rSensColIndex_ = rightCol;
	}

	// Shoot alien bullets
	for (int i = 1; i < 12; ++i) {
		if (bulletArray_[i]->getActivity() == false) {
			if ((rng_() % 2) == 1) {
				// Select alien that fired
				while (1) {
					int alienIndex = rng_() % 55;
					if (aliens_.at(alienIndex)->getActivity() == true) {
						cVector2 alienPos = aliens_.at(alienIndex)->getPos();
						bulletArray_[i]->setPos(alienPos);
						bulletArray_[i]->setActivity(true);
						break;
					}
				}

			}
		}
	}
	
	// Move the bullets
	if (bulletArray_[0]->getActivity() == true)
	{
		bulletArray_[0]->translate(0,-playerBulletVel_*tickSpeedMult);
	}
	for (int i = 1; i < 12; ++i)
	{
		if (bulletArray_[i]->getActivity() == true)
		{
			bulletArray_[i]->translate(0,alienBulletVel_*tickSpeedMult);
		}
	}

	std::forward_list<cCollPair> collPairList = world_->checkColls();
	while (collPairList.empty() == false)
	{
		collHandler(collPairList.front());
		collPairList.pop_front();
	}
	return eStateAction::NONE;
}

void cMainState::renderState (SDL_Renderer* renderer, double timeLag)
{
//	if (showDebug_ == true)
		world_->drawDebugWorld(renderer);
}
