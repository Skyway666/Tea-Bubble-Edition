#pragma once
#ifndef __j1ENTITIES_H__
#define __j1ENTITIES_H__

#include "j1Module.h"
#include "j1Collisions.h"
#include "p2List.h"
#include "p2PQueue.h"
#include "Chronometer.h"



enum ENTITY_TYPES
{
	PLAYER,
	GROUND_ENEMY,
	AIR_ENEMY,
	COIN
};

struct Player_Initial_Inf
{
	fPoint speed_modifier;
	fPoint walljump_speed;
	float gravity;
	float jump_time;
	float slide_time;
	float walljump_time;
	iPoint pathfinding_distance;
};

struct Flying_Enemy_Initial_Inf
{
	float side_fly_time; //Could be initialized with an argument
	int agro_distance; //Could be initialized with an argument	
	int tired_distance; //Could be initialized with an argument
	int idle_speed; //Could be initialized with an argument	
	fPoint speed_modifier; //Could be initialized with an argument //Could be initialized with an argument
};
struct Ground_Enemy_Initial_Inf
{
	fPoint speed_modifier;
	float gravity;
	float jump_time;
};

struct Entity_info
{
	iPoint position;
	ENTITY_TYPES type;
};

class Entity;

class j1Entities : public j1Module
{
public:

	j1Entities();
	~j1Entities();

	bool Awake(pugi::xml_node& conf); //Load initial information of all entities
	bool Start(); //Load enemy textures
	bool Update(float dt); //Update all entities in "entities"
	bool CleanUp(); //Called when app is closed
	void EraseEntities(); // To erase all entities left on the map
	void EraseEntity(Entity* entity); // To erase a single entity
	void ErasePlayer(); 
	void OnCollision(Collider* c1, Collider* c2); //Called by the callback of "j1Collisions", should iterate over all entities and call their OnCollision method if the collider is theirs
	Entity* AddEntity(ENTITY_TYPES type, int x, int y); //Adds an enemy to the list with a certain type and position (should return a pointer to the enemy... or not)
	void Add_waiting_entity(ENTITY_TYPES type, int x, int y); //Adds an entity that will be spawned when "spawn_waiting_entities" is called
	void Spawn_waiting_entities(); //Spawn entities in the waiting queue
	void Clear_waiting_list(); //Clears waiting_queue
	void Load_entities(pugi::xml_node& data);
	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	Player* player = nullptr; //Really special entity

private:

	p2List<Entity*> entities;
	SDL_Texture* enemy_sprites = nullptr;
	SDL_Texture* player_sprites = nullptr;
	SDL_Texture* coin_sprites = nullptr;
	p2PQueue<Entity_info> waiting_queue;
	Chronometer do_logic;

	Player_Initial_Inf p_ini_inf;
	Flying_Enemy_Initial_Inf fe_ini_inf;
	Ground_Enemy_Initial_Inf ge_ini_inf;

};


#endif // __j1ENTITIES_H__