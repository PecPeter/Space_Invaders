#ifndef COLLHANDLER_HPP
#define COLLHANDLER_HPP

#include <iostream>
#include <list>

#include "2D-Engine/collision/collPair.hpp"
#include "2D-Engine/entity/entity.hpp"

#include "entityMasks.hpp"
#include "usrPtrObjects.hpp"

// This function determines which collision handler function to use
void collHandler (const cCollPair& collPair);

// Collision functions for the dynamic objects
// Ship collision functions
void collShipWithWall (cEntity& ship, cEntity& walls,
		const std::list<sCollPairInfo>& collList);
void collShipWithAlienBullet (cEntity& ship, cEntity& alienBullet);

// Ship bullet collision functions
void collShipBulletWithWall (cEntity& shipBullet);
void collShipBulletWithAlienBullet (cEntity& shipBullet, cEntity& alienBullet);
void collShipBulletWithShield (cEntity& shipBullet, cEntity& shield,
		const std::list<sCollPairInfo>& collList);
void collShipBulletWithAlien (cEntity& shipBullet, cEntity& alien);

// Alien bullet collision functions
void collAlienBulletWithShip (cEntity& alienBullet, cEntity& ship);
void collAlienBulletWithShipBullet (cEntity& alienBullet, cEntity& shipBullet);
void collAlienBulletWithShield (cEntity& alienBullet, cEntity& shield,
		const std::list<sCollPairInfo>& collList);
void collAlienBulletWithGroundSensor (cEntity& alienBullet);

// Alien collision functions
void collAlienWithShipBullet (cEntity& alien, cEntity& shipBullet);
void collAlienWithShield (cEntity& shield,
		const std::list<sCollPairInfo>& collList);
void collAlienWithGroundSensor (cEntity& alien);

// Alien sensor collision functions
void collAlienSensorWithWall (cEntity& alienSensor,
		const std::list<sCollPairInfo>& collList);

#endif
