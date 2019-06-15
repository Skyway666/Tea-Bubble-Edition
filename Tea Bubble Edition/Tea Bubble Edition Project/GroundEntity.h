#pragma once
#include "Entity.h"
#include "Chronometer.h"

class GroundEntity : public Entity
{
public:
	GroundEntity(int, int);
	~GroundEntity();

	float gravity;
	iPoint contact;
	bool allowtime = true;
	bool jumping = false;
	float jump_time;
	Chronometer jump_timer;
	virtual void Jump(float dt) {};
};