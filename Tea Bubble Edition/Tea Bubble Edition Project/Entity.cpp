#include "j1App.h"
#include "Entity.h"
#include "j1Collisions.h"
#include "j1Render.h"

Entity::Entity(int x, int y)
{
	position.x = x;
	position.y = y;
}

Entity::~Entity()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* Entity::GetCollider() const
{
	return collider;
}

void Entity::Draw(SDL_Texture* sprites)
{
	  if(animation != nullptr)
		App->render->Blit(sprites, position.x, position.y, scale, true, &(animation->GetCurrentFrame()), flip);
}