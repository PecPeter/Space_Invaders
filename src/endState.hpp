#ifndef ENDSTATE_HPP
#define ENDSTATE_HPP

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

#include "2D-Engine/engineMetrics.hpp"
#include "2D-Engine/gameState.hpp"
#include "2D-Engine/controllers/cntrlKb.hpp"
#include "2D-Engine/renderFunctions.hpp"

#include "gameStateTypes.hpp"

enum class eKbAction_EndState {
	RESTART,
	QUIT
};

class cEndState : public cGameState {
	public:
		cEndState (void** interStateInfo);
		~cEndState (void);
	private:
		void handleState (SDL_Event& event);
		int updateState (double tickRate, void** interStateInfo);
		void renderState (SDL_Renderer* renderer, double timeLag);

		cCntrlKb<eKbAction_EndState> cntrlKb_;
		std::vector<eKbAction_EndState> kbActions_;
		TTF_Font* font_;
};

#endif
