#pragma once
#include "Entity.h"
#include "Animation.h"
#include "j1Collisions.h"
#include "p2Point.h"
#include "j1Entities.h"
#include "j1Collisions.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "GroundEntity.h"
#include "Chronometer.h"

class Player : public GroundEntity
{
public:
	Player(int x, int y, Player_Initial_Inf initial_inf);
	~Player();
	
	void Update(float dt);	
	void ManagePhysics(float dt);
	void OnCollision(Collider* collider);
	void Save(pugi::xml_node& data);
	void Load(pugi::xml_node& data);
	void Pause();
	void Resume();
	
	float player_x_displacement; //Used by parallax

	// Used by WillCollide
	bool walljumping = false;
	bool sliding = false;
	bool StickToWall = false;
	iPoint pathfinding_distance;
	int coins = 0;
	int lvl2coins = 0;
	int lvl2points = 0;
	int points = 0;
	int lives = 3;

private:

	void WallSlide();
	void Jump(float dt);
	void Slide(float dt);

	//Transitates from map 1 to map 2
	 bool want_transition_1 = false;

	void Animation_Loading();
	SDL_Rect rect_after_sliding;	

	float player_height_before_sliding;
	bool dead = false;
	bool win = false;
	bool godmode = false;
	bool making_transition = false; //Bool so code that has to be executed when player dies is executed just once
	int jcontact = 0; // for Jump() function

	bool player_reset_current_map; // If this bool is true, the player will reset its position in the current map

	iPoint sprite_distance;
	fPoint walljump_speed;
	
	float slide_time;
	float walljump_time;

	Chronometer slide_timer;
	Chronometer walljump_timer;
	Chronometer Win_timer;

	Animation idle;
	Animation run;
	Animation jump;
	Animation fall;
	Animation death;
	Animation slide;
	Animation wallslideright;
	Animation wallslideleft;
};
