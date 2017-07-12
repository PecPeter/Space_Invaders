#include "collHandler.hpp"

void collHandler (const cCollPair& collPair) {
	cEntity& ent1 = collPair.ent1(),
		   & ent2 = collPair.ent2();

	int entMask1 = ent1.getMask(),
		entMask2 = ent2.getMask();

	if (entMask1 == SHIP_MASK && entMask2 == WALL_MASK)
		collShipWithWall(ent1,ent2,collPair.getCollisions());
	else if (entMask1 == SHIP_MASK && entMask2 == ALIEN_BULLET_MASK)
		collShipWithAlienBullet(ent1,ent2);
	else if (entMask1 == SHIP_BULLET_MASK && entMask2 == WALL_MASK)
		collShipBulletWithWall(ent1);
	else if (entMask1 == SHIP_BULLET_MASK && entMask2 == ALIEN_BULLET_MASK)
		collShipBulletWithAlienBullet(ent1,ent2);
	else if (entMask1 == SHIP_BULLET_MASK && entMask2 == SHIELD_MASK)
		collShipBulletWithShield(ent1,ent2,collPair.getCollisions());
	else if (entMask1 == SHIP_BULLET_MASK && entMask2 == ALIEN_MASK)
		collShipBulletWithAlien(ent1,ent2);
	else if (entMask1 == ALIEN_BULLET_MASK && entMask2 == SHIP_MASK)
		collAlienBulletWithShip (ent1,ent2);
	else if (entMask1 == ALIEN_BULLET_MASK && entMask2 == SHIP_BULLET_MASK)
		collAlienBulletWithShipBullet(ent1,ent2);
	else if (entMask1 == ALIEN_BULLET_MASK && entMask2 == SHIELD_MASK)
		collAlienBulletWithShield (ent1,ent2,collPair.getCollisions());
	else if (entMask1 == ALIEN_BULLET_MASK && entMask2 == BULLET_SENSOR_MASK)
		collAlienBulletWithGroundSensor(ent1);
	else if (entMask1 == ALIEN_MASK && entMask2 == SHIP_BULLET_MASK)
		collAlienWithShipBullet(ent1,ent2);
	else if (entMask1 == ALIEN_MASK && entMask2 == SHIELD_MASK)
		collAlienWithShield(ent2,collPair.getCollisions());
	else if (entMask1 == ALIEN_MASK && entMask2 == ALIEN_END_SENSOR_MASK)
		collAlienWithAlienEndSensor(ent1);
	else if (entMask1 == ALIEN_SENSOR_MASK && entMask2 == WALL_MASK)
		collAlienSensorWithWall(ent1,collPair.getCollisions());
}

// Ship collision functions
void collShipWithWall (cEntity& ship, cEntity& walls,
		const std::list<sCollPairInfo>& collList) {
	cVector2 translation = zeroVector2;
	for (auto& collPairInfo : collList)
	{
		if (collPairInfo.overlap_ != zeroVector2)
			translation = collPairInfo.overlap_;
	}
	ship.translate(translation);
}

void collShipWithAlienBullet (cEntity& ship, cEntity& alienBullet) {
	static_cast<sShipInfo*>(ship.getUsrPtr())->numShips -= 1;
	ship.setPos(320,330);
	alienBullet.setNodeActivity(0,false);
	(static_cast<cEntity*>(alienBullet.getUsrPtr()))->setState(ALIEN_IDLE);
}

// Ship bullet collision functions
void collShipBulletWithWall (cEntity& shipBullet) {
	shipBullet.setNodeActivity(0,false);
	static_cast<cEntity*>(shipBullet.getUsrPtr())->setState(SHIP_IDLE);
}

void collShipBulletWithAlienBullet (cEntity& shipBullet, cEntity& alienBullet) {
	shipBullet.setNodeActivity(0,false);
	alienBullet.setNodeActivity(0,false);
	static_cast<cEntity*>(shipBullet.getUsrPtr())->setState(SHIP_IDLE);
	static_cast<cEntity*>(alienBullet.getUsrPtr())->setState(ALIEN_IDLE);
}

void collShipBulletWithShield (cEntity& shipBullet, cEntity& shield,
		const std::list<sCollPairInfo>& collList) {
	shipBullet.setNodeActivity(0,false);
	static_cast<cEntity*>(shipBullet.getUsrPtr())->setState(SHIP_IDLE);
	// Subtract health from the shield node that was hit, deactivate
	// it if its too low
	for (const auto& itr : collList) {
		if (itr.collType_ == eCollType::CONTACT ||
				itr.collType_ == eCollType::COLLISION) {
			sShieldHealth* shieldHealth =
				static_cast<sShieldHealth*>(shield.getUsrPtr());
			int health = shieldHealth->nodeHealth[itr.entNodeId2_];
			health > 1 ? --health : health = 0;
			if (health == 0) {
				shield.setNodeActivity(itr.entNodeId2_,false);
			}
			shieldHealth->nodeHealth[itr.entNodeId2_] = health;
		}
	}
}

void collShipBulletWithAlien (cEntity& shipBullet, cEntity& alien) {
	shipBullet.setNodeActivity(0,false);
	alien.setNodeActivity(0,false);
	int alienCol = (alien.getId()-100)/5;
	sAlienInfo* tmpPtr = static_cast<sAlienInfo*>(alien.getUsrPtr());
	tmpPtr->totalAliens -= 1;;
	tmpPtr->numAlienCol[alienCol] -= 1;
	static_cast<cEntity*>(shipBullet.getUsrPtr())->setState(SHIP_IDLE);
}

// Alien bullet collision functions
void collAlienBulletWithShip (cEntity& alienBullet, cEntity& ship) {
	collShipWithAlienBullet(ship,alienBullet);
}

void collAlienBulletWithShipBullet (cEntity& alienBullet, cEntity& shipBullet) {
	collShipBulletWithAlienBullet(shipBullet,alienBullet);
}

void collAlienBulletWithShield (cEntity& alienBullet, cEntity& shield,
		const std::list<sCollPairInfo>& collList) {
	alienBullet.setNodeActivity(0,false);
	// Subtract health from the shield node that was hit, deactivate
	// it if its too low
	for (const auto& itr : collList) {
		if (itr.collType_ == eCollType::CONTACT ||
				itr.collType_ == eCollType::COLLISION) {
			sShieldHealth* shieldHealth =
				static_cast<sShieldHealth*>(shield.getUsrPtr());
			int health = shieldHealth->nodeHealth[itr.entNodeId2_];
			health > 1 ? --health : health = 0;
			if (health == 0) {
				shield.setNodeActivity(itr.entNodeId2_,false);
			}
			shieldHealth->nodeHealth[itr.entNodeId2_] = health;
		}
	}
}

void collAlienBulletWithGroundSensor (cEntity& alienBullet) {
	alienBullet.setNodeActivity(0,false);
	static_cast<cEntity*>(alienBullet.getUsrPtr())->setState(ALIEN_IDLE);
}

// Alien collision functions
void collAlienWithShipBullet (cEntity& alien, cEntity& shipBullet) {
	collShipBulletWithAlien(shipBullet,alien);
}

void collAlienWithShield (cEntity& shield,
		const std::list<sCollPairInfo>& collList) {
	for (auto& itr : collList) {
		if (itr.collType_ == eCollType::COLLISION) {
			shield.setNodeActivity(itr.entNodeId2_,false);
		}
	}
}

void collAlienWithAlienEndSensor (cEntity& alien) {
	static_cast<sAlienInfo*>(alien.getUsrPtr())->endState = eEndState::GAME_OVER;
}

// Alien sensor collision functions
void collAlienSensorWithWall (cEntity& alienSensor,
		const std::list<sCollPairInfo>& collList) {
	sCollPairInfo collPairInfo = collList.front();
	sAlienInfo* alienInfo = static_cast<sAlienInfo*>(alienSensor.getUsrPtr());
	if (collPairInfo.collType_ == eCollType::CONTACT ||
			collPairInfo.collType_ == eCollType::COLLISION) {
		alienInfo->alienDir *= -1.0;
		alienInfo->alienVertDir = 1.0;
	}
}
