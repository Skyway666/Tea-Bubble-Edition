#include "CupDispenser.h"
#include "Player.h"



CupDispenser::CupDispenser(iPoint position): StaticEntity(position) {

	collider.w = 100;
	collider.h = 200;
}


CupDispenser::~CupDispenser() {
}

void CupDispenser::Update() {
}

void CupDispenser::Take(Player * player) {
	player->object.type = EMPTY_CUP;
}
