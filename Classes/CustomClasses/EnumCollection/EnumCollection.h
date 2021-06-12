#pragma once

enum CharType
{ 
	NONE   = -1,
	PLAYER =  0,
	ENEMY  =  1
};
enum CharAnim
{
	//add
	MOVE_ANIM,
	PHYSICAL_ATTACK_ANIM,
	MAGIC_ATTACK_ANIM_1,
	MAGIC_ATTACK_ANIM_2,
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
	PHYSICAL_ATTACK,
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