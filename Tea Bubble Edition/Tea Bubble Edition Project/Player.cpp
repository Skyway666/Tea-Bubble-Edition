#include "Player.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Input.h"

#include"StaticEntity.h"


Player::Player() {
}


Player::~Player() {
}

void Player::Update() {

	// The player is the mouse
	App->input->GetMousePosition(position.x, position.y);

	// Manage hovered object iterating "StaticEntities" list
	// Call "Player events"
}

void Player::Draw(SDL_Texture* draw_tex, float scale) {
	if (object.type == NONE_OBJECT_TYPE) 
		return;

	App->render->Blit(draw_tex, position.x, position.y, scale, false, &(object.current_animation.GetCurrentFrame()));
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
