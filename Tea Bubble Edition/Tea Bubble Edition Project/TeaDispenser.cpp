#include "TeaDispenser.h"
#include "j1Entities.h"
#include "j1Render.h"
#include "j1App.h"
#include "p2Log.h"


TeaDispenser::TeaDispenser(iPoint position): StaticEntity(position) {
	collider.x = cup_position.x = 120;
	collider.y = cup_position.y = 200;
	collider.w = collider.h = 300;
	
	idle.PushBack({ 1,1,1,1 }); // No art yet!
	current_animation = &idle;

	filled_cup.PushBack({ 0,300, 200, 300 });

	type = TEA_DISPENSER;

	// Tea dispenser holds no cup when initialized
	cup.type = NONE_OBJECT_TYPE;
}


TeaDispenser::~TeaDispenser(){
}

void TeaDispenser::Update() {

}
void TeaDispenser::Draw(SDL_Texture * draw_tex, float scale) {

	// Draw the static thing
	StaticEntity::Draw(draw_tex, scale);

	// Draw cup
	// Calculate position
	iPoint holding_cup_global_position = position + cup_position;
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

void TeaDispenser::FillCup() {

	// TODO: Start timer that will end up filling the cup, and animation accordingly (or just animation and check Finished())
	cup.current_animation = filled_cup;
	
}
