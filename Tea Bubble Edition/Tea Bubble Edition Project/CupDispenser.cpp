#include "CupDispenser.h"
#include "j1Entities.h"




CupDispenser::CupDispenser(iPoint position): StaticEntity(position) {

	collider.w = 200;
	collider.h = 400;

	type = CUP_DISPENSER;

	// Static entity: Inicialization
	idle.PushBack({ 0,0, 200, 400 });
	current_animation = &idle;

	// Given object: Inicialization
	empty_cup.current_animation.PushBack({ 0,0, 200, 300 });
	empty_cup.type = EMPTY_CUP;
}


CupDispenser::~CupDispenser() {
}
void CupDispenser::Take(Player * player) {
	player->object = empty_cup;
}
