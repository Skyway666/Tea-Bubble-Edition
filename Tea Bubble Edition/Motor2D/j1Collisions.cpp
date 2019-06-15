#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "j1Map.h"
#include "Pathfinding.h"
#include "j1Entities.h"
#include "Player.h"
#include "GroundEnemy.h"

j1Collisions::j1Collisions()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_BONE] = true;
	matrix[COLLIDER_GOD][COLLIDER_BONE] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_DEADLY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY_GROUND] = true;
	matrix[COLLIDER_ENEMY_GROUND][COLLIDER_PATH] = true;
	matrix[COLLIDER_ENEMY_GROUND][COLLIDER_WALKABLE] = true;
	matrix[COLLIDER_COIN][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_COIN] = true;
	matrix[COLLIDER_COIN][COLLIDER_GOD] = true;
	matrix[COLLIDER_GOD][COLLIDER_COIN] = true;
}

// Destructor
j1Collisions::~j1Collisions()
{}



bool j1Collisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

// Called before render is available
bool j1Collisions::Update(float dt)
{
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty and wall colliders
		if (colliders[i] == nullptr || colliders[i]->type == COLLIDER_WALL)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty and wall colliders
			if (colliders[k] == nullptr || colliders[k]->type == COLLIDER_WALL)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{

				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);


				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);

			}
		}
	}

	UpdateGroundPath();
	DebugDraw();

	return true;
}

void j1Collisions::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha, false);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha, false);
			break;
		case COLLIDER_GOD: // black
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 0, alpha, false);
			break;
		case COLLIDER_WALL: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha, false);
			break;
		case COLLIDER_PIT: // pink
			App->render->DrawQuad(colliders[i]->rect, 243, 64, 147, alpha, false);
			break;
		case COLLIDER_DEADLY: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha, true);
			break;
		case COLLIDER_ENEMY_GROUND: // cian
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha, true);
			break;
		case COLLIDER_BONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha, true);
			break;
		case COLLIDER_WALKABLE: // purple
			App->render->DrawQuad(colliders[i]->rect, 101, 31, 180, alpha, true);
			break;
		case COLLIDER_PATH: // brown
			App->render->DrawQuad(colliders[i]->rect, 15, 50, 85, alpha, true);
			break;
		case COLLIDER_COIN: // orange
			App->render->DrawQuad(colliders[i]->rect, 255, 128, 0, alpha, true);
			break;
		}
	}
}

// Called before quitting
bool j1Collisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Collisions::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback, uint lenght, uint height, uint column_height)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback, lenght, height, column_height);
			break;
		}
	}

	return ret;
}

void j1Collisions::Erase_Non_Player_Colliders()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->type != COLLIDER_PLAYER && colliders[i]->type != COLLIDER_GOD)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	if (r.y + r.h > rect.y && r.y < rect.y + rect.h && r.x + r.w > rect.x && r.x < rect.x + rect.w)
		return true;
	else
		return false;
}

void Collider::WillCollide(GroundEntity* entity, float dt)
{
	const SDL_Rect r = entity->collider->rect;

	if (r.y + r.h > rect.y && r.y < rect.y + rect.h && r.x < rect.x + rect.w + entity->speed_modifier.x * dt && r.x + r.w > rect.x) // Will collide left
		entity->contact.x = 1;
	if (r.y + r.h > rect.y && r.y < rect.y + rect.h && r.x + r.w > rect.x - entity->speed_modifier.x * dt && r.x < rect.x + rect.w) // Will collide right
		entity->contact.x = 2;
	if (r.y < rect.y + rect.h && r.y + r.h >(rect.y - entity->gravity * dt) && r.x + r.w > rect.x && r.x < rect.x + rect.w) // Will collide ground
	{
		entity->contact.y = 1;

		if (entity->type == GROUND_ENEMY)
		{
			GroundEnemy* enemy = (GroundEnemy*)entity;
			enemy->height = height; // Set height to ground enemies
		}
	}
	if (r.y + r.h > rect.y && r.y < rect.y + rect.h + entity->speed_modifier.y * dt && r.x + r.w > rect.x && r.x < rect.x + rect.w) // Will collide top
		entity->contact.y = 2;
}

void Collider::WillCollidePit(GroundEnemy* entity, float dt)
{
	if (entity->collider != nullptr)
	{
		const SDL_Rect r = entity->collider->rect;

		// Will collide ground && contact.y == 1
		if ((r.y < rect.y + rect.h && r.y + r.h > rect.y - entity->gravity * dt && r.x + r.w > rect.x && r.x < rect.x + rect.w) && entity->contact.y == 1)
		{
			if (lenght < 4)
			{
				// Prevent jumps upon landing
				if ((!entity->flip && entity->collider->rect.x + entity->collider->rect.w / 2 > rect.x) || (entity->flip
					&& entity->collider->rect.x + entity->collider->rect.w / 2 < rect.x + rect.w))
				{
					if (entity->just_landed)
					{
						if (entity->jump_timer.Read() != 0)
							entity->jump_x = lenght * App->map->data.tile_width / entity->jump_timer.Read();

						entity->jump_timer.Reset();
						entity->jumping = true;
					}
					entity->just_landed = false;
				}
			}
			else if (lenght >= 4)
			{
				entity->front_of_unwalkable = true;
			}
		}
	}
}

void Collider::WillCollideWall(GroundEnemy* entity, float dt)
{
	if (entity->collider != nullptr)
	{ 
		const SDL_Rect r = entity->collider->rect;

		// Will collide left or Will collide right and contact.y == 1
		if ((r.y + r.h > rect.y && r.y < rect.y + rect.h && r.x < rect.x + rect.w + App->map->data.tile_width && r.x + r.w > rect.x)
			|| (r.y + r.h > rect.y && r.y < rect.y + rect.h && r.x + r.w > rect.x - App->map->data.tile_width && r.x < rect.x + rect.w))
		{
			if (column_height <= 2 && entity->height - height <= 3 && entity->height - height > 0 && entity->contact.y == 1) // Check if height is bigger than 0 just in case
			{
				entity->jumping_wall = true;
				entity->jump_timer.Reset();
				entity->jumping = true;
			}
		}
	}
}

bool j1Collisions::WillCollideAfterSlide(Player* entity, float dt) const
{
	const SDL_Rect r = entity->collider->rect;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty and player colliders
		if (colliders[i] == nullptr || colliders[i]->type == COLLIDER_NONE || colliders[i]->type == COLLIDER_PLAYER)
			continue;

		if ((colliders[i]->type == COLLIDER_WALL || (colliders[i]->type == COLLIDER_PIT && entity->collider->type == COLLIDER_GOD))
			&& r.y + r.h > colliders[i]->rect.y && r.y < colliders[i]->rect.y + colliders[i]->rect.h + App->map->data.tile_height
			&& r.x + r.w > colliders[i]->rect.x && r.x < colliders[i]->rect.x + colliders[i]->rect.w)
			return true;
	}

	return false;
}

void j1Collisions::ManageGroundCollisions(GroundEntity* entity, float dt)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty and non-wall colliders
		if (colliders[i] == nullptr || colliders[i]->type == COLLIDER_NONE || colliders[i]->type == COLLIDER_PLAYER || colliders[i]->type == COLLIDER_BONE
			|| colliders[i]->type == COLLIDER_ENEMY || colliders[i]->type == COLLIDER_GOD)
			continue;

		if (entity->collider != nullptr && entity->collider->type != COLLIDER_GOD && colliders[i]->type == COLLIDER_WALL)
		{
			colliders[i]->WillCollide(entity, dt);

			if (entity->collider->CheckCollision(colliders[i]->rect)) // In case the entity somehow passes thorugh a wall
			{
				if (entity->type == ENTITY_TYPES::PLAYER)
				{
					Player* player = (Player*)entity;

					if (player->flip && !player->walljumping && !player->StickToWall)
						player->position.x += player->speed_modifier.x * dt;
					else if (!player->flip && !player->walljumping && !player->StickToWall)
						player->position.x -= player->speed_modifier.x * dt;
					else if (player->walljumping && player->speed.x > 0 && !player->StickToWall)
						player->position.x -= player->speed_modifier.x * dt;
					else if (player->walljumping && player->speed.x < 0 && !player->StickToWall)
						player->position.x += player->speed_modifier.x * dt;
				}
				else
				{
					if (entity->flip)
						entity->position.x += App->map->data.tile_width / 2;
					else if (!entity->flip)
						entity->position.x -= App->map->data.tile_width / 2;
				}
			}
		}
	}
}

void j1Collisions::EnemyJump(GroundEnemy* entity, float dt)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// Skip empty, non-wall and non-pit colliders
		if (colliders[i] == nullptr || !(colliders[i]->type == COLLIDER_PIT || colliders[i]->type == COLLIDER_WALL))
			continue;

		if (colliders[i]->type == COLLIDER_PIT)
			colliders[i]->WillCollidePit(entity, dt);
		else if (colliders[i]->type == COLLIDER_WALL)
			colliders[i]->WillCollideWall(entity, dt);
	}
}

void j1Collisions::UpdateGroundPath()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// Skip empty, non-walkable and non-path colliders
		if (colliders[i] == nullptr || !(colliders[i]->type == COLLIDER_PATH || colliders[i]->type == COLLIDER_WALKABLE))
			continue;

		iPoint distance;

		if (App->entities->player->collider != nullptr)
		{
			distance.x = abs(colliders[i]->rect.x - (App->entities->player->position.x + App->entities->player->collider->rect.w / 2));
			distance.y = abs(colliders[i]->rect.y - (App->entities->player->position.y + App->entities->player->collider->rect.h / 2));
		}

		if (distance.x <= App->entities->player->pathfinding_distance.x && distance.y <= App->entities->player->pathfinding_distance.y)
			colliders[i]->type = COLLIDER_PATH;
		else
			colliders[i]->type = COLLIDER_WALKABLE;
	}
}