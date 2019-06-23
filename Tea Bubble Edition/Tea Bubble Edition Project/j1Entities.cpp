#include "j1Entities.h"
#include "p2Log.h"
#include "CupDispenser.h"




j1Entities::j1Entities(): j1Module() {

}


j1Entities::~j1Entities() {
}

bool j1Entities::Awake(pugi::xml_node& config) {
	return true;
}

bool j1Entities::Start() {
	CreatePlayer();
	return true;
}

bool j1Entities::PreUpdate() {
	return true;
}

bool j1Entities::Update(float dt) {

	for (uint i = 0; i < static_entities.count(); ++i) {
		static_entities[i]->Update();
		//static_entities[i]->Draw()

		//if (debug_draw) static_entities[i]->DebugDraw();
	}

	// Update Player
	if(player){
		player->Update();
		//player->Draw();

		if (debug_draw) player->DebugDraw();
	}

	return true;
}

bool j1Entities::PostUpdate() {
	return true;
}

bool j1Entities::CleanUp() {
	return true;
}

void j1Entities::CreatePlayer() {

	if (player)
		delete player;

	player = new Player();
}

StaticEntity* j1Entities::CreateStaticEntity(STATIC_ELEMENT_TYPE element) {

	StaticEntity* new_entity = nullptr;
	switch (element) {
		case CUP_DISPENSER:
			new_entity = new CupDispenser();
			break;
		case TEA_DISPENSER:
			// Create child of Static Entity
			break;
		case CONDIMENT_DISPENSER:
			// Create child of Static Entity
			break;
	}

	static_entities.add(new_entity);
	return new_entity;
}

void j1Entities::PlayerEvent(PLAYER_EVENT event) {

	// From player, get "hovered_enitity", and call right function depending on the event
	
	switch (event) {
		case GRIP:
			
			break;
		//...
	}
}
