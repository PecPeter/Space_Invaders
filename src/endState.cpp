#include "endState.hpp"

cEndState::cEndState (void** interStateInfo):
		cGameState(eStateAction::NONE,eStateAction::REMOVE_STATE),
		font_(nullptr) {
	cntrlKb_.addCommand(eKbAction_EndState::RESTART,SDLK_r);
	cntrlKb_.addCommand(eKbAction_EndState::QUIT,SDLK_ESCAPE);

	font_ = TTF_OpenFont("./assets/fonts/TerminusTTF.ttf",12);
}

cEndState::~cEndState (void) {
	TTF_CloseFont(font_);
	font_ = nullptr;
}

void cEndState::handleState (SDL_Event& event) {
	switch (event.type) {
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			cntrlKb_.checkCommand(event.key,&kbActions_);
			break;
		default:
			break;
	}
}

int cEndState::updateState (double tickRate, void** interStateInfo) {
	cntrlKb_.removeRepeatCommands(&kbActions_);
	if (kbActions_.size() > 0) {
		for (auto& itr : kbActions_) {
			switch (itr) {
				case eKbAction_EndState::RESTART: {
					return eStateAction::MAIN_STATE;
					break;
				}
				case eKbAction_EndState::QUIT: {
					return eStateAction::REMOVE_STATE;
					break;
				}
				default:
					break;
			}
		}
	}
	return eStateAction::NONE;
}

void cEndState::renderState (SDL_Renderer* renderer, double timeLag) {
	cVector2 renderPos(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
	cVector4 renderCol(255,255,255,0);
	drawStrU(renderer,font_,renderPos,"GAME OVER",renderCol);
	drawStrD(renderer,font_,renderPos,
			"Press ESC to exit, or r to restart",cVector4(255,255,255,0));
}
