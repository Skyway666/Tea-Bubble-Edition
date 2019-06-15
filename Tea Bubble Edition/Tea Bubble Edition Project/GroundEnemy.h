#pragma once
#include "Entity.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Input.h"
#include "Pathfinding.h"
#include "j1Entities.h"
#include "Animation.h"
#include "Player.h"
#include "GroundEntity.h"
#include "Chronometer.h"


class GroundEnemy : public GroundEntity
{
public:
	GroundEnemy(int x, int y, Ground_Enemy_Initial_Inf initial_inf);
	~GroundEnemy();
	Animation run;
	Animation jump;

	void Update(float dt, bool do_logic);
	void OnCollision(Collider* collider);
	void Pause();
	void Resume();
	void Save(pugi::xml_node& data);
	void Load(pugi::xml_node& data);

	void ManagePhysics(float dt);

	bool front_of_unwalkable = false;
	bool turn = false;
	uint height; // To determine wether if the enemy jumps upon reching a wall or not
	float jump_x = 0; // X speed when jumping
	bool just_landed = false;
	bool jumping_wall = false;
	float initial_jump_time;
	float last_jump;

private:

	void Jump(float dt);
	void Exec_idle();
	void Exec_attack();

	int frames = 0;
	int time = 0;
	bool is_idle = true; // bool to know when the enemy should stop ideling and follow the player
	int player_pos; // -1 if the player is on the left, 1 if the player is on the right
	int last_contact_y;
	bool spawned = false;
};