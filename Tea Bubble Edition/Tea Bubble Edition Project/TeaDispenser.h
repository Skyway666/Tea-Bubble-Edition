#pragma once
#include "StaticEntity.h"
#include "Player.h"

class TeaDispenser :
	public StaticEntity {
public:
	TeaDispenser(iPoint position);
	~TeaDispenser();

	void Draw(SDL_Texture* draw_tex, float scale = 1);

	void Leave(Player* player);
	void Take(Player* player);


	Animation idle;

	iPoint cup_position; // Relative to parent, CENTER
	Object cup;
};

