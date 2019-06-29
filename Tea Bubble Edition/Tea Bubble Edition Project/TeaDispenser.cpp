#include "TeaDispenser.h"
#include "j1Entities.h"
#include "j1Render.h"
#include "j1App.h"


TeaDispenser::TeaDispenser(iPoint position): StaticEntity(position) {

	collider.w = collider.h = 300;
	cup_position.x = 120;
	cup_position.y = 200;
	
	idle.PushBack({ 1,1,1,1 }); // No art yet!
	current_animation = &idle;

	type = TEA_DISPENSER;

	// Tea dispenser holds no cup when initialized
	cup.type = NONE_OBJECT_TYPE;
}


TeaDispenser::~TeaDispenser(){
}

void TeaDispenser::Draw(SDL_Texture * draw_tex, float scale) {

	// Draw the static thing
	StaticEntity::Draw(draw_tex, scale);

	// Draw cup

	// Calculate position
	iPoint holding_cup_global_position = { collider.x, collider.y }; 
	holding_cup_global_position += cup_position;

	// Draw
	App->entities->DrawObject(holding_cup_global_position, cup);
}

void TeaDispenser::Leave(Player * player) {
	cup = player->object;
}

void TeaDispenser::Take(Player* player) {
	player->object = cup;
	cup.type = NONE_OBJECT_TYPE;
}