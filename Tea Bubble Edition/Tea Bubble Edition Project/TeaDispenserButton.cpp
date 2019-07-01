#include "TeaDispenserButton.h"
#include "p2Point.h"
#include "TeaDispenser.h"
#include "j1Entities.h"


TeaDispenserButton::TeaDispenserButton(iPoint position): StaticEntity(position) {
	collider.x = position.x;
	collider.y = position.y;
	collider.w = collider.h = 50;

	idle.PushBack({ 1,1,1,1 }); // No art yet!
	current_animation = &idle;

	type = TEA_DISPENSER_BUTTON;
}


TeaDispenserButton::~TeaDispenserButton() {
}

void TeaDispenserButton::Take(Player * player) {
	if(linked_tea_dispenser)
		linked_tea_dispenser->FillCup();
}
