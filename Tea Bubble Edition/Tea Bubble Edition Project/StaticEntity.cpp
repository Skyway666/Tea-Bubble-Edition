#include "StaticEntity.h"
#include "j1Render.h"
#include "j1App.h"



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
	App->render->DrawQuad(collider, 255, 255, 255, 255, true, false);
}
