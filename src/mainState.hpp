#ifndef MAINSTATE_HPP
#define MAINSTATE_HPP

#include <iostream>
#include <vector>

#include "2D-Engine/gameState.hpp"
#include "2D-Engine/collision/collDebugDrawer.hpp"
#include "2D-Engine/controllers/cntrlKb.hpp"
#include "2D-Engine/collision/collWorld.hpp"
#include "2D-Engine/entity/entity.hpp"
#include "2D-Engine/entity/entityNode.hpp"

#include "gameStateTypes.hpp"
#include "collHandler.hpp"

enum class eKbAction
{
	M_LEFT,
	M_RIGHT,
	SHOOT,
	QUIT
};

class cMainState : public cGameState
{
	public:
		cMainState (void);
		~cMainState (void);
	private:
		void handleState (SDL_Event& event);
		int updateState (double tickRate, void** interStateInfo);
		void renderState (SDL_Renderer* renderer, double timeLag);

		cCntrlKb<eKbAction> cntrlKb_;
		std::vector<eKbAction> kbActions_;

		cCollWorld* world_;
		cCollDebugDrawer* drawer_;

		cEntity* ship_,
			   * walls_,
			   * shield1_,
			   * shield2_,
			   * shield3_,
			   * shield4_;

		// Entitiy vector for the bullets (player and aliens can only shoot once)
		// First bullet in the array is for the player shot
		cEntity* bulletArray_[12];
		double playerBulletVel_,
			   alienBulletVel_;
};

#endif
