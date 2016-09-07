#include "mainState.hpp"

cMainState::cMainState (void): cGameState(eStateAction::NONE,
		eStateAction::REMOVE_STATE), world_(nullptr), ship_(nullptr)
{
	cntrlKb_.addCommand(eKbAction::M_LEFT,SDLK_a);
	cntrlKb_.addCommand(eKbAction::M_RIGHT,SDLK_d);
	cntrlKb_.addCommand(eKbAction::SHOOT,SDLK_s);
	cntrlKb_.addCommand(eKbAction::QUIT,SDLK_ESCAPE);

	world_ = new cCollWorld(eBroadphaseType::GENERAL);
	drawer_ = new cCollDebugDrawer;
	world_->setDebugDraw(drawer_);

	// Create the ship entity
	cPosComp shipPos(0,0,0);

	cCollAabb shipShape(10,10);

	cEntityNode shipNode(0,shipPos,cCollComp(shipShape));

	ship_ = world_->createEntity(0,cPosComp(320,330,0),shipNode,
			eEntityType::DYNAMIC,SHIP_MASK);
	world_->addCollMask(SHIP_MASK,SHIP_COLL_MASK);
	
	// Create the walls
	cPosComp wallPos1(-50,0,0),
			 wallPos2(50,0,0),
			 wallPos3(0,-100,0),
			 wallPos4(0,100,0);

	cCollAabb vertWall(5,100),
			  horzWall(50,5);

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

	// Create the bullets
	cCollAabb bulletShape(1.0,1.0);

	cPosComp bulletPos(0.0,0.0,0);
	cPosComp bulletPos1(300,100,0);

	cCollComp bulletCollComp(bulletShape);
	
	cEntityNode bulletNode(0,bulletPos,bulletCollComp);

	for (int i = 0; i < 12; ++i)
	{
		bulletArray_[i] = world_->createEntity(6+i,bulletPos1,bulletNode,
				eEntityType::DYNAMIC,BULLET_MASK);
		bulletArray_[i]->setNodeActivity(0,false);
	}
	world_->addCollMask(BULLET_MASK,BULLET_COLL_MASK);
	playerBulletVel_ = 5;
	alienBulletVel_ = 5;
}

cMainState::~cMainState (void)
{
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

int cMainState::updateState (double tickRate, void** interStateInfo)
{
	// Move the player ship, shoot or quit
	if (kbActions_.size() > 0) {
		for (auto& kbCommand : kbActions_) {
			switch (kbCommand) {
				case eKbAction::M_LEFT:
					ship_->translate(-1,0);
					break;
				case eKbAction::M_RIGHT:
					ship_->translate(1,0);
					break;
				case eKbAction::SHOOT:
					bulletArray_[0]->setNodeActivity(0,true);
					bulletArray_[0]->setPos(ship_->getPos());
					break;
				case eKbAction::QUIT:
					return eStateAction::REMOVE_STATE;
					break;
				default:
					break;
			}
		}
	}

	// Move the aliens
	
	// Shoot alien bullets
	
	// Move the bullets
	if (bulletArray_[0]->getActivity() == true)
	{
		bulletArray_[0]->translate(0,-playerBulletVel_);
	}
	for (int i = 1; i < 12; ++i)
	{
		if (bulletArray_[i]->getActivity() == true)
		{
			bulletArray_[i]->translate(0,alienBulletVel_);
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
	world_->drawDebugWorld(renderer);
}
