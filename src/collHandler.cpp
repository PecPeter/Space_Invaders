#include "collHandler.hpp"

void collHandler (const cCollPair& collPair)
{
	cEntity& ent1 = collPair.ent1(),
		   & ent2 = collPair.ent2();
	int id1 = ent1.getId(),
		id2 = ent2.getId();

	if (id1 == 0 && id2 == 1)
		collShipWithWalls(ent1,ent2,collPair.getCollisions());
}

void collShipWithWalls (cEntity& ship, cEntity& walls,
		const std::list<sCollPairInfo>& collList)
{
	cVector2 translation = zeroVector2;
	for (auto& collPairInfo : collList)
	{
		if (collPairInfo.overlap_ != zeroVector2)
			translation = collPairInfo.overlap_;
	}
	ship.translate(translation);
}
