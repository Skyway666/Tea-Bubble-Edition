#include "Player.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Input.h"


Player::Player() {
}


Player::~Player() {
}

void Player::Update() {

	// The player is the mouse
	App->input->GetMousePosition(position.x, position.y);
}

void Player::Draw(SDL_Texture* draw_tex, float scale) {
	if (object.type == NONE_OBJECT_TYPE) 
		return;

	App->render->Blit(draw_tex, position.x, position.y, scale, false, &(object.current_animation.GetCurrentFrame()));
}

void Player::DebugDraw() {
	switch (object.type) {
		case NONE_OBJECT_TYPE:
		
		break;
	}
}
