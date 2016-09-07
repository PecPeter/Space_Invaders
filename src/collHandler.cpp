#include "collHandler.hpp"

void collHandler (const cCollPair& collPair)
{
	cEntity& ent1 = collPair.ent1(),
		   & ent2 = collPair.ent2();

	int entMask1 = ent1.getMask(),
		entMask2 = ent2.getMask();

	if (entMask1 == SHIP_MASK && entMask2 == WALL_MASK)
		collShipWithWall(ent1,ent2,collPair.getCollisions());
	else if (entMask1 == BULLET_MASK && entMask2 == WALL_MASK)
		collBulletWithWall(ent1);
	else if (entMask1 == BULLET_MASK && entMask2 == SHIELD_MASK)
		collBulletWithShield(ent1,ent2,collPair.getCollisions());
}

void collShipWithWall (cEntity& ship, cEntity& walls,
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

void collBulletWithWall (cEntity& bullet)
{
	// If a bullet collides with a wall, deactivate the bullet
	bullet.setNodeActivity(0,false);
}

// Ent1 = bullet, Ent2 = shield
void collBulletWithShield (cEntity& bullet, cEntity& shield,
		const std::list<sCollPairInfo>& collList)
{
	// Deactivate the bullet
	bullet.setNodeActivity(0,false);

	// Subtract health from the shield node that was hit, deactivate
	// it if its too low
	for (const auto& itr : collList)
	{
		if (itr.collType_ == eCollType::CONTACT ||
				itr.collType_ == eCollType::COLLISION)
		{
			sShieldHealth* shieldHealth =
				static_cast<sShieldHealth*>(shield.getUsrPtr());
			int health = shieldHealth->nodeHealth[itr.entNodeId2_];
			health > 1 ? --health : health = 0;
			if (health == 0)
			{
				shield.setNodeActivity(itr.entNodeId2_,false);
			}
			shieldHealth->nodeHealth[itr.entNodeId2_] = health;
		}
	}
}
