#include "2D-Engine/gameState.hpp"
#include "2D-Engine/engine.hpp"

#include "gameStateTypes.hpp"
#include "endState.hpp"
#include "mainState.hpp"

void stateChangeHandler (int stateChange, cGameState** statePntr,
		void** interStateInfo) {
	switch (stateChange) {
		case eStateAction::NONE:
			break;
		case eStateAction::REMOVE_STATE:
			delete (*statePntr);
			(*statePntr) = nullptr;
			break;
		case eStateAction::MENU_STATE:
			break;
		case eStateAction::MAIN_STATE:
			delete (*statePntr);
			(*statePntr) = new cMainState;
			break;
		case eStateAction::END_STATE:
			delete (*statePntr);
			(*statePntr) = new cEndState(interStateInfo);
		default:
			break;
	}
}

int main (void) {
	cEngine engine;

	char title[] = "SPACE INVADERS";

	cGameState* statePntr = nullptr;
	statePntr = new cMainState;

	if (engine.init(640,480,title,&stateChangeHandler,&statePntr)
			== false) {
		std::cerr << "Could not initialize engine properly. " <<
			"Now exiting." << std::endl;
		return 1;
	}
	engine.mainLoop();
	engine.quit();

	return 0;
}
