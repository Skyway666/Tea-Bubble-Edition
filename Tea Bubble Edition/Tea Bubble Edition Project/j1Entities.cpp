#include "j1Entities.h"
#include "p2Log.h"
#include "CupDispenser.h"
#include "j1App.h"
#include "j1Input.h"




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

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug_draw = !debug_draw;

	return true;
}

bool j1Entities::Update(float dt) {

	// Update entities and manage hover
	for (uint i = 0; i < static_entities.count(); ++i) {
		static_entities[i]->Update();
		//static_entities[i]->Draw()

		if (debug_draw) static_entities[i]->DebugDraw();

		ManageHover(static_entities[i]);
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

StaticEntity* j1Entities::CreateStaticEntity(STATIC_ELEMENT_TYPE element, iPoint position) {

	StaticEntity* new_entity = nullptr;
	switch (element) {
		case CUP_DISPENSER:
			new_entity = new CupDispenser(position);
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

void j1Entities::ManageHover(StaticEntity * entity_check) {

	// Player is hovering object
	if (CheckCollision(entity_check->collider, player->position) && entity_check != player->hovered_entity) {
		entity_check->Hover();
		player->hovered_entity = entity_check;
	}
	// Player is unhovering object
	if (!CheckCollision(entity_check->collider, player->position) && entity_check == player->hovered_entity) {
		entity_check->UnHover();
		player->hovered_entity = nullptr;
	}

}
