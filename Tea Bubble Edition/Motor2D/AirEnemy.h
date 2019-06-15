#pragma once
#include "Entity.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Input.h"
#include "Pathfinding.h"
#include "j1Entities.h"
#include "Animation.h"
#include "Chronometer.h"
#include "Player.h"

class AirEnemy : public Entity
{
public:
	AirEnemy(int x, int y, Flying_Enemy_Initial_Inf initial_inf);
	~AirEnemy();
	Animation idle;

	void Update(float dt, bool do_logic);
	void OnCollision();
	void Pause();
	void Resume();
	void Save(pugi::xml_node& data);
	void Load(pugi::xml_node& data);


private:
	bool is_idle; //bool to know when the enemy should stop ideling and follow the player
	Chronometer side_fly_timer; //Timer to mesure when enemy should be moving
	Chronometer chase_timer; // Timer to mesure how much time the player is being chased
	p2DynArray<iPoint> path_to_follow;//Path that the enemy must follow
	int next_tile; //Tile that the enemy has to reach
	int idle_speed; //speed at which the enemy idles	
	bool returning; //Bool to know if enemy should be returning to the starting point
	bool home_path_found;//Bool to find path home just one time

    float side_fly_time; //Time that the enemy spends flying from one side to the other
	int agro_distance; //Distance at which enemy starts following the player(IN TILES)
	iPoint initial_tile; //Spawn tile of the enemy (IN TILES)
	int tired_distance; //Distance from the initial point at which the player stops being chased (IN TILES)

	void Exec_idle();
	void Exec_path();
	void Find_path_player();
	void Find_path_home();
	int distance_to_player(); //Distance to player in tiles
	int distance_to_start(); //Distance to starting point in tiles
	
};