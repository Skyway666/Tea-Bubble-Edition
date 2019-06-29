#include "Player.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Input.h"

#include"StaticEntity.h"
#include "j1Entities.h"


Player::Player() {
}


Player::~Player() {
}

void Player::Update() {

	// The player is the mouse
	App->input->GetMousePosition(position.x, position.y);

	// Call "Player events" when player clicks


	if (App->input->GetMouseButtonDown(1) == KEY_DOWN) {
		if (hovered_entity)
			hovered_entity->Take(this);
	}
	if (App->input->GetMouseButtonDown(1) == KEY_UP) {
		if (hovered_entity)
			hovered_entity->Leave(this);
		object.type = NONE_OBJECT_TYPE;
	}
}

void Player::Draw(SDL_Texture* draw_tex, float scale) {
	if (object.type == NONE_OBJECT_TYPE) 
		return;

	iPoint centered_pos = { object.current_animation.GetCurrentFrame().w/2, object.current_animation.GetCurrentFrame().h/2};
	centered_pos = position - centered_pos;

	App->entities->DrawObject(centered_pos, object);
}

void Player::DebugDraw() {

	SDL_Rect debug_rect;
	debug_rect.x = position.x - 25;
	debug_rect.y = position.y - 25;
	debug_rect.h = 50;
	debug_rect.w = 50;

	int r = 0, g = 0, b = 0, a = 0;

	switch (object.type) {
		case NONE_OBJECT_TYPE:
			r = 255;
			g = 0;
			b = 0;
			a = 255;
			break;
		case EMPTY_CUP:
			r = 0;
			g = 255;
			b = 255;
			a = 255;
			break;
	}

	App->render->DrawQuad(debug_rect, r, g, b, a, true, false);
}
