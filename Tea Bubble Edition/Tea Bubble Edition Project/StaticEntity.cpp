#include "StaticEntity.h"
#include "j1Render.h"
#include "j1App.h"



StaticEntity::StaticEntity() {
}


StaticEntity::~StaticEntity() {
}

void StaticEntity::Update() {

}

void StaticEntity::Draw(SDL_Texture* draw_tex, float scale) {

	if (current_animation != nullptr) {
		App->render->Blit(draw_tex, collider.x, collider.y, scale, false, &(current_animation->GetCurrentFrame()));
	}
}

void StaticEntity::DebugDraw() {
	App->render->DrawQuad(collider, 255, 255, 255, 255, true, false);
}
