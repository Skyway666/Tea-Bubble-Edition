#pragma once
#include "Entity.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Entities.h"
#include "Animation.h"
#include "Chronometer.h"
#include "Player.h"

class Coins : public Entity
{
public:

	Coins(int x, int y);
	~Coins();

	void Update(float dt, bool dologic);
	void OnCollision(Collider* collider);
	void Pause();
	void Resume();
	void Save(pugi::xml_node& data);
	void Set_id(int _id);
	int Get_id();

private:

	Animation anim;
	bool collected = false;
	bool given_points = false;
	bool pa_started = false; // pa = play animation
	bool animation_played = false;
	Chronometer play_animation;
	int id; //What coin is
};

