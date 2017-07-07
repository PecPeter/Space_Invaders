#ifndef ENTITYMASKS_HPP
#define ENTITYMASKS_HPP

// Entity masks
const int SHIP_MASK = 2;				// 000000010
const int WALL_MASK = 4;				// 000000100
const int SHIP_BULLET_MASK = 8;			// 000001000
const int ALIEN_BULLET_MASK = 16;		// 000010000
const int SHIELD_MASK = 32;				// 000100000
const int ALIEN_MASK = 64;				// 001000000
const int ALIEN_SENSOR_MASK = 128;		// 010000000
const int GROUND_SENSOR_MASK = 256;		// 100000000

// Collision masks
const int SHIP_COLL_MASK = 20;			// 0000101-0
const int WALL_COLL_MASK = 138;			// 010001-10
const int SHIP_BULLET_COLL_MASK = 116;	// 00111-100
const int ALIEN_BULLET_COLL_MASK = 298;	// 1001-1010
const int SHIELD_COLL_MASK = 88;		// 001-11000
const int ALIEN_COLL_MASK = 296;		// 10-101000
const int ALIEN_SENSOR_COLL_MASK = 4;	// 0-0000100
const int GROUND_SENSOR_COLL_MASK = 80;	// -01010000

#endif
