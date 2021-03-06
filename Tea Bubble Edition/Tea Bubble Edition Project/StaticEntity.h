#ifndef __StaticEntity__
#define __StaticEntity__

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "Animation.h"

enum STATIC_ELEMENT_TYPE;
class Player;

class StaticEntity {
public:
	// Methods
	StaticEntity() {};
	StaticEntity(iPoint position);
	~StaticEntity();	
	virtual void Update();
	virtual void Draw(SDL_Texture* draw_tex, float scale = 1);

	virtual void Take(Player* player){}
	virtual void Leave(Player* player){}
	virtual void Hover(){}
	virtual void UnHover(){}

	void DebugDraw();

	// Variable

	iPoint position;
	Animation* current_animation = nullptr;
	STATIC_ELEMENT_TYPE type;
	SDL_Rect collider; // Contains position
};

#endif