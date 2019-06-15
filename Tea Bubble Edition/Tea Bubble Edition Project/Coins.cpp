#include "Coins.h"
#include "j1Textures.h"
#include "j1Scene.h"



Coins::Coins(int x, int y) : Entity(x, y)
{
	position.x = x;
	position.y = y;

	scale = 0.3;
	type = ENTITY_TYPES::COIN;

	anim.PushBack({ 32, 0, 147, 405 });
	anim.PushBack({ 261, 0, 147, 405 });
	anim.PushBack({ 484, 0, 212, 405 });

	anim.speed = 0.2;
	int W = anim.GetCurrentFrame().w * scale;
	int H = anim.GetCurrentFrame().h * scale;
	collider = App->collision->AddCollider({ x, y, W, H }, COLLIDER_COIN, App->entities);

	animation = &anim;
}


Coins::~Coins()
{
}


void Coins::Update(float dt, bool dologic)
{
	if (given_points && !App->pause)
	{
		scale -= 0.01;
		position.y -= 20;
		position.x -= 24;
		collider->SetPos(position.x, position.y);
		
		if (!pa_started)
		{
			App->audio->PlayFx(3);
			play_animation.Start(0.6);
			pa_started = true;
		}
		if (play_animation.IsOver() && !animation_played)
		{
			App->scene->coin_animation = true;
			to_delete = true;
			animation_played = true;
		}
	}

	animation->speed = 0.2 * dt;
}

void Coins::OnCollision(Collider* collider)
{
	if (collider->type == COLLIDER_PLAYER || collider->type == COLLIDER_GOD)
	{
		if (!given_points)
		{
			App->entities->player->coins++;
			App->entities->player->points++;
			given_points = true;
		}
	}
}

void Coins::Pause()
{
	animation->Reset();
	play_animation.Pause();
}

void Coins::Resume()
{
	play_animation.StartAfterPause();
	play_animation.ResetPause();
}

void Coins::Save(pugi::xml_node& data)
{
	data.append_attribute("id") = id;
}

void Coins::Set_id(int _id)
{
	id = _id;
}

int Coins::Get_id()
{
	return id;
}