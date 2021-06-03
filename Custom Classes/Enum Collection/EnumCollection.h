#pragma once

enum CharType
{ 
	NONE   = -1,
	PLAYER =  0,
	ENEMY  =  1
};
enum CharAnim
{
	PHYSICAL_ATTACK_ANIM,
	MAGIC_ATTACK_ANIM,
	GUARD_ANIM,
	DODGE_ANIM,
	DAMAGE_ANIM,
	IDLE_ANIM,
	DEAD_ANIM
};
enum Direction
{ 
	LEFT  = -1, 
	ZERO  =  0,
	RIGHT =  1
};
enum StoneType 
{
	PHYSICAL_ATTACK = 0,
	MAGIC_ATTACK,
	GUARD,
	DODGE
};
enum StoneTier 
{
	NORMAL,
	RARE,
	UNIQUE,
	EPIC
};

enum UpdateType {
	Selection,
	Result
};