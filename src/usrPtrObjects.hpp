#ifndef USRPTROBJECTS_HPP
#define USRPTROBJECTS_HPP

#include <vector>

enum class eEndState {
	NONE,
	NEXT_ROUND,
	GAME_OVER
}; 

struct sShieldHealth
{
	int nodeHealth[4] = {10,10,10,10};
};

struct sAlienInfo
{
	double alienBaseSpeed = 0.5,
		   alienSpeedMult = 1.0;
	// LEFT = -1, RIGHT = 1
	double alienDir = 1.0;
	double alienVertOffset = 5.0;
	double alienVertDir = 0.0;
	eEndState endState = eEndState::NONE;

	int numAlienCol[11] = {5,5,5,5,5,5,5,5,5,5,5};
	int totalAliens = 55;
};

struct sShipInfo {
	int numShips = 3;
};

const int SHIP_IDLE = 0,
	  	  SHIP_MOVE_LEFT = 1,
		  SHIP_MOVE_RIGHT = 2,
		  SHIP_SHOOT = 3,
		  ALIEN_IDLE = 0,
		  ALIEN_MOVE_LEFT = 1,
		  ALIEN_MOVE_RIGHT = 2,
		  ALIEN_SHOOT = 3;

#endif
