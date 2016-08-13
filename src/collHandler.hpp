#ifndef COLLHANDLER_HPP
#define COLLHANDLER_HPP

#include <list>

#include "2D-Engine/collision/collPair.hpp"
#include "2D-Engine/entity/entity.hpp"

// This function determines which collision handler function to use
void collHandler (const cCollPair& collPair);

// Make a collision handler function for the different entities
void collShipWithWalls (cEntity& ship, cEntity& walls,
		const std::list<sCollPairInfo>& collList);

#endif
