#include "StaticEntity.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Entities.h"



StaticEntity::StaticEntity(iPoint position): position(position){
}


StaticEntity::~StaticEntity() {
}

void StaticEntity::Update() {

}

void StaticEntity::Draw(SDL_Texture* draw_tex, float scale) {

	// Blit texture with animation
	SDL_Rect* animation_rect = nullptr;
	if (current_animation)
		animation_rect = &(current_animation->GetCurrentFrame());

	App->render->Blit(draw_tex, position.x, position.y, scale, false,animation_rect);
}

void StaticEntity::DebugDraw() {

	int r = 0, g = 0, b = 0, a = 120;

	switch (type) {
	case CUP_DISPENSER:
		r = 0;
		g = 255;
		b = 255;
		break;
	case TEA_DISPENSER:
		r = 0;
		g = 255;
		b = 0;
		break;
	case TEA_DISPENSER_BUTTON:
		r = 120;
		g = 120;
		b = 0;
	}
	App->render->DrawQuad(collider, r, g, b, a, true, false);
}
