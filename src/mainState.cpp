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

	ship_ = world_->createEntity(0,cPosComp(320,240,0),shipNode,
			eEntityType::DYNAMIC);
	
	// Create the walls
	cPosComp wallPos1(-20,0,0),
			 wallPos2(20,0,0),
			 wallPos3(0,-40,0),
			 wallPos4(0,40,0);

	cCollAabb horzWall(5,80),
			  vertWall(40,5);

	std::vector<cEntityNode> walls;
	walls.push_back(cEntityNode(0,wallPos1,cCollComp(horzWall)));
	walls.push_back(cEntityNode(1,wallPos2,cCollComp(horzWall)));
	walls.push_back(cEntityNode(2,wallPos3,cCollComp(vertWall)));
	walls.push_back(cEntityNode(3,wallPos4,cCollComp(vertWall)));

	walls_ = world_->createEntity(1,cPosComp(320,240,0),walls);

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

	cPosComp posShield1(282.5,100,0),
			 posShield2(307.5,100,0),
			 posShield3(332.5,100,0),
			 posShield4(357.5,100,0);

	shield1_ = world_->createEntity(2,posShield1,shieldNodes);
	shield2_ = world_->createEntity(3,posShield2,shieldNodes);
	shield3_ = world_->createEntity(4,posShield3,shieldNodes);
	shield4_ = world_->createEntity(5,posShield4,shieldNodes);
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
					break;
				case eKbAction::QUIT:
					return eStateAction::REMOVE_STATE;
					break;
				default:
					break;
			}
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
