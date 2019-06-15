#pragma once
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animation.h"
#include "PugiXml\src\pugixml.hpp"

//Parent class to all the entities

struct SDL_Texture;
struct Collider;
enum ENTITY_TYPES;
class Entity //: public Path
{
protected:
	Animation* animation = nullptr;

public:
	fPoint position; // Top left position
	iPoint center; // Used for pathfinding
	fPoint speed_modifier;
	fPoint speed;
	Collider* collider = nullptr; // Collider
	float scale; // Scale to blit
	ENTITY_TYPES type; // Knowing which type of enemy
	bool flip = false; // Flip to blit
	bool to_delete = false;

public:

	Entity(int x, int y);
	virtual ~Entity();

	const Collider* GetCollider() const;

	void Draw(SDL_Texture* sprites); // Draw and update collider position

	virtual void Update(float dt, bool do_logic) {}; // Update enemy logic
	virtual void ManagePhysics(float dt) {}; // Manage Physics of grounded entities
	virtual void Pause() {}; // Stop Chronometers and animations when paused
	virtual void Resume() {}; // Restart Chronometers when resumed
	virtual void OnCollision(Collider* collider) {}; // Every entity has its own "OnCollision"
	virtual void Save(pugi::xml_node& data) {}; //Every enemy saves his own data
	virtual void Load(pugi::xml_node& data) {}; //Every enemy saves his own data

};

#endif // __ENEMY_H__