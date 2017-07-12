#ifndef MAINSTATE_HPP
#define MAINSTATE_HPP

#include <ctime>
#include <iostream>
#include <cmath>
#include <vector>

#include "2D-Engine/engineMetrics.hpp"
#include "2D-Engine/gameState.hpp"
#include "2D-Engine/collision/collDebugDrawer.hpp"
#include "2D-Engine/controllers/cntrlKb.hpp"
#include "2D-Engine/collision/collWorld.hpp"
#include "2D-Engine/entity/entity.hpp"
#include "2D-Engine/entity/entityNode.hpp"
#include "mathLib/mtrand.hpp"

#include "entityMasks.hpp"
#include "usrPtrObjects.hpp"
#include "gameStateTypes.hpp"
#include "collHandler.hpp"

enum class eKbAction
{
	M_LEFT,
	M_RIGHT,
	SHOOT,
	TOGGLE_DEBUG,
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

		cEntity* leftSensor_,
			   * rightSensor_,
			   * groundSensorBullet_,
			   * groundSensorAlien_;
		std::vector<cEntity*> aliens_;

		sShieldHealth shield1Hp_,
					  shield2Hp_,
					  shield3Hp_,
					  shield4Hp_;

		// Entitiy vector for the bullets (player and aliens can only shoot once)
		// First bullet in the array is for the player shot
		cEntity* bulletArray_[12];
		double playerBulletVel_,
			   alienBulletVel_;
		sAlienInfo alienInfo_;
		double shipVel_;
		sShipInfo shipInfo_;

		bool showDebug_;
		int lSensColIndex_,
			rSensColIndex_;
		MTRand_int32 rng_;
};

#endif
