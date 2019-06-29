#include "StaticEntity.h"
#include "j1Render.h"
#include "j1App.h"
#include "j1Entities.h"



StaticEntity::StaticEntity(iPoint position){
	collider.x = position.x;
	collider.y = position.y;
}


StaticEntity::~StaticEntity() {
}

void StaticEntity::Update() {

}

void StaticEntity::Draw(SDL_Texture* draw_tex, float scale) {

	// Blit texture with animation
	if (current_animation) 
		App->render->Blit(draw_tex, collider.x, collider.y, scale, false, &(current_animation->GetCurrentFrame()));
	// Blit all the texture
	else
		App->render->Blit(draw_tex, collider.x, collider.y, scale, false, nullptr);

}

void StaticEntity::DebugDraw() {

	int r = 0, g = 0, b = 0, a = 0;

	switch (type) {
	case CUP_DISPENSER:
		r = 0;
		g = 255;
		b = 255;
		a = 255;
		break;
	case TEA_DISPENSER:
		r = 0;
		g = 255;
		b = 0;
		a = 255;
		break;
	}
	App->render->DrawQuad(collider, r, g, b, a, true, false);
}
