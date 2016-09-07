#ifndef ENTITYMASKS_HPP
#define ENTITYMASKS_HPP

// Entity masks
const int SHIP_MASK = 2;			// 000010
const int WALL_MASK = 4;			// 000100
const int BULLET_MASK = 8;			// 001000
const int SHIELD_MASK = 16;			// 010000
const int ALIEN_MASK = 32;			// 100000

// Collision masks
const int SHIP_COLL_MASK = 12;		// 001100
const int WALL_COLL_MASK = 42;		// 101010
const int BULLET_COLL_MASK = 62;	// 111110
const int SHIELD_COLL_MASK = 40;	// 101000
const int ALIEN_COLL_MASK = 30;		// 011110

#endif
