#ifndef COLLHANDLER_HPP
#define COLLHANDLER_HPP

#include <list>

#include "2D-Engine/collision/collPair.hpp"
#include "2D-Engine/entity/entity.hpp"

#include "entityMasks.hpp"
#include "usrPtrObjects.hpp"

// This function determines which collision handler function to use
void collHandler (const cCollPair& collPair);

// Make a collision handler function for the different entities
void collShipWithWall (cEntity& ship, cEntity& walls,
		const std::list<sCollPairInfo>& collList);
void collBulletWithWall (cEntity& bullet);
void collBulletWithShield (cEntity& bullet, cEntity& shield,
		const std::list<sCollPairInfo>& collList);

#endif
