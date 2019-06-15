#include "Player.h"
#include "j1Transition.h"


Player::Player(int x, int y, Player_Initial_Inf initial_inf) : GroundEntity(x, y)
{
	Animation_Loading();

	jump_time = initial_inf.jump_time / 100;
	slide_time = initial_inf.slide_time / 100;
	walljump_time = initial_inf.walljump_time / 100;
	speed_modifier.y = initial_inf.speed_modifier.y;
	speed_modifier.x = initial_inf.speed_modifier.x;
	walljump_speed.y = initial_inf.walljump_speed.y;
	walljump_speed.x = initial_inf.walljump_speed.x;
	gravity = initial_inf.gravity;
	pathfinding_distance.x = initial_inf.pathfinding_distance.x * App->map->data.tile_width;
	pathfinding_distance.y = initial_inf.pathfinding_distance.y * App->map->data.tile_height;
	
	scale = 0.3;
	type = ENTITY_TYPES::PLAYER;
	SDL_Rect r{ 0, 0, 481, 547 };
	SDL_Rect collider_rect{ x, y, r.w * 0.2, r.h * 0.2 };
	contact.x = 0;
	contact.y = 0;
	collider = App->collision->AddCollider(collider_rect, COLLIDER_PLAYER, App->entities);
	animation = &idle;
	lives = 3;
}


Player::~Player()
{

}

void Player::Update(float dt)
{
	//Calculate center of the player
	center.x = position.x + (481 * scale) / 2;
	center.y = position.y + (547 * scale) / 2;

	if (App->max_punctuation < points)
		App->max_punctuation = points;

	idle.speed = 0.8 * dt;
	run.speed = 0.4 * dt;
	jump.speed = 0.4 * dt;
	fall.speed = 0.2 * dt;
	death.speed = 0.8 * dt;
	slide.speed = 0.4 * dt;
	wallslideright.speed = 0.1 * dt;
	wallslideleft.speed = 0.1 * dt;
	App->map->bone_animation.speed = 0.2 * dt;

	if (contact.y != 0 && !godmode)
		speed.y = speed_modifier.y;

	if (godmode)
		collider->type = COLLIDER_GOD;
	else
		collider->type = COLLIDER_PLAYER;

	player_x_displacement = App->map->data.player_starting_value.x - position.x;

	if (contact.y == 1 && !sliding)
	{
		animation = &idle;
		fall.Reset();
	}
	else if (contact.y != 1)
		animation = &fall;

	speed.x = 0;

	if (dead)
	{
		animation = &death;
		if (App->map->map == 0)
		{
			coins = 0;
			points = 0;
		}
		else if (App->map->map == 1)
		{
			coins = lvl2coins;
			points = lvl2points;
		}

		if (animation->Finished())
		{
			if (lives > 0)
			{
				if (!App->transition->transitioning)
				{
					App->transition->Make_transition(&player_reset_current_map);
				}

			}
			else 
			{
				if(!App->transition->transitioning)
				{ 
					App->transition->Make_transition(&App->scene->want_load_main_menu, &App->scene->want_unload_HUD, &App->scene->want_unload_map);
				}
			}

		}
	}

	if (player_reset_current_map)
	{
		position.x = App->map->data.player_starting_value.x;
		position.y = App->map->data.player_starting_value.y - 5;
		center.x = position.x + (481 * scale) / 2;
		center.y = position.y + (547 * scale) / 2;
		App->entities->EraseEntities();
		App->entities->Spawn_waiting_entities();
		death.Reset();
		dead = false;
		player_reset_current_map = false;
	}

	if (want_transition_1)
	{
		App->scene->Change_to_map(1);
		position.x = App->map->data.player_starting_value.x;
		position.y = App->map->data.player_starting_value.y - gravity * 4;
		speed.y = speed.x = 0;
		want_transition_1 = false;
	}

	if (Win_timer.IsOver() && !dead)
	{
		// God mode
		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			if (godmode)
				godmode = false;
			else
				godmode = true;
		}

		if (godmode && App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
			position.x += 2000;
		else if (godmode && App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
			position.x -= 2000;

		// Sliding
		if (!godmode && App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && contact.y == 1)
		{
			slide_timer.Reset();
			sliding = true;
		}
		else if (godmode && App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			speed.y = speed_modifier.y;

		// Moving right
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !sliding)
		{
			flip = false;

			if (contact.y == 1)
				animation = &run;
			if (contact.x != 2)
				speed.x = speed_modifier.x;
		}

		// Moving left
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !sliding)
		{
			flip = true;

			if (contact.y == 1)
				animation = &run;
			if (contact.x != 1)
				speed.x = -speed_modifier.x;
		}

		// Stop moving if both A and D are pressed
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			speed.x = 0;
			if (animation == &run)
				animation = &idle;
		}

		WallSlide();

		// Jumping
		if (!godmode && App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && !sliding)
		{
				if (contact.y == 1)
				{
					jump_timer.Reset();
					jumping = true;
				}
				else if (StickToWall)
				{
					walljump_timer.Reset();
					walljumping = true;
				}
		}
		else if (godmode && App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			speed.y = -speed_modifier.y;

		if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			if (App->map->map != 0)
			{
				App->scene->Change_to_map(0);
			}
			App->entities->EraseEntities();
			App->entities->Spawn_waiting_entities();
			position.x = App->map->data.player_starting_value.x;
			position.y = App->map->data.player_starting_value.y - gravity * 2;
			coins = 0;
			points = 0;
			lives = 3;
		}
		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		{
			App->entities->EraseEntities();
			App->entities->Spawn_waiting_entities();
			position.x = App->map->data.player_starting_value.x;
			position.y = App->map->data.player_starting_value.y - gravity * 2;
			coins = 0;
			points = 0;
			lives = 3;
		}
	}

	Jump(dt);
	Slide(dt);

	if (sliding && contact.x != 1 && flip)
		speed.x = -1.5 * speed_modifier.x;
	else if (sliding && contact.x != 2 && !flip)
		speed.x = 1.5 * speed_modifier.x;


	if (!walljumping)
		position.x += speed.x * dt;

	if (contact.y != 1 && !godmode)
	{
		if (StickToWall)
			position.y += gravity / 2 * dt;
		else if (contact.y != 1)
			position.y += gravity * dt;
	}

	if (godmode)
	{
		position.y += speed.y * dt;
		speed.y = 0;
	}

	StickToWall = false;
	contact.x = 0;
	contact.y = 0;

	//Put collider next to player (Should be done in "draw")
	if (collider != nullptr)
	{
		if (!sliding)
			collider->SetPos(position.x + 30, position.y + 30);
		else
		{
			collider->SetPos(position.x + 30, position.y + 547 * 0.2 - App->map->data.tile_height - 1 + 50);
		}
	}

	//If player is below the map level, kill the player
	if (position.y > App->map->data.height* App->map->data.tile_height && !dead)
	{
		App->audio->PlayFx(5);
		lives--;
		dead = true;
	}
}

void Player::WallSlide()
{
	if (contact.x == 2 && contact.y != 1 && contact.y != 2 && App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !jumping)
	{
		StickToWall = true;
		animation = &wallslideright;
		flip = false;
	}
	else if (contact.x == 1 && contact.y != 1 && contact.y != 2 && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !jumping)
	{
		StickToWall = true;
		animation = &wallslideleft;
		flip = false;
	}
}

void Player::Jump(float dt)
{
	// jump
	if (jumping)
	{
		if (jump_timer.IsOver() && contact.y == 1)
		{
			jump_timer.Start(jump_time);
			contact.y = 0;
			App->audio->PlayFx(1);
			fall.Reset();
		}

		if (!jump_timer.IsOver() && contact.y == 0)
		{
			animation = &jump;
			position.y -= speed.y * dt;
		}
		else
		{
			jumping = false;
			jump_timer.Reset();
			jump.Reset();
		}
	}
	// wall jump
	else if (walljumping)
	{
		fall.Reset();

		if (walljump_timer.IsOver() && contact.x != 0)
		{
			walljump_timer.Start(walljump_time);
			jcontact = contact.x;
			contact.x = 0;
			App->audio->PlayFx(1);
		}

		if (!walljump_timer.IsOver() && contact.x == 0)
		{
			animation = &jump;
			position.y -= walljump_speed.y * dt;

			if (jcontact == 1)
			{
				position.x += walljump_speed.x * dt;
				flip = true;
			}
			else if (jcontact == 2)
				position.x -= walljump_speed.x * dt;
		}
		else
		{
			walljump_timer.Reset();
			walljumping = false;
			jump.Reset();
		}

		if (contact.y == 1)
		{
			walljump_timer.Reset();
			walljumping = false;
			jump.Reset();
		}
	}
}

void Player::Slide(float dt)
{
	if (sliding)
	{
		if (slide_timer.IsOver() && allowtime)
		{
			slide_timer.Start(slide_time);
			collider->SetSize(collider->rect.w + 3, App->map->data.tile_height - 50);
			allowtime = false;
			player_height_before_sliding = position.y;
			App->audio->PlayFx(2);
		}
		if (!slide_timer.IsOver())
		{
			animation = &slide;
			rect_after_sliding.x = position.x;
			rect_after_sliding.y = player_height_before_sliding;
			rect_after_sliding.h = 547 * 0.2;
			rect_after_sliding.w = 481 * 0.2;
		}
		else if (App->collision->WillCollideAfterSlide(this, dt) && contact.x == 0)
		{
			slide_timer.Start(slide_time);
		}
		else
		{
			sliding = false;
			slide_timer.Reset();
			collider->SetSize(481 * 0.2, 547 * 0.2);
			allowtime = true;
			if (contact.y == 1)
				position.y = player_height_before_sliding - 3;
		}
	}
}

void Player::Animation_Loading()
{
	int row = 0;

	sprite_distance.x = 548;
	sprite_distance.y = 482;

	// dying
	for (int i = 0; i < 10; i++)
		death.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * row, 547, 481 });

	death.loop = false;
	death.speed = 0.8;
	row++;

	// idle animation
	for (int i = 0; i < 10; i++)
		idle.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * row, 547, 481 });

	idle.speed = 0.8;
	row++;

	// running
	for (int i = 0; i < 8; i++)
		run.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * row, 547, 481 });

	row++;

	// sliding
	for (int i = 0; i < 10; i++)
		slide.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * row, 547, 481 });

	row++;

	// jumping
	for (int i = 0; i < 8; i++)
		jump.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * row, 547, 481 });

	jump.loop = false;

	// wall slide left

	wallslideleft.PushBack({ 1 + sprite_distance.x * 8, 1 + sprite_distance.y * row, 547, 481 });
	wallslideleft.PushBack({ 1 + sprite_distance.x * 9, 1 + sprite_distance.y * row, 547, 481 });

	wallslideleft.speed = 0.1;
	row++;

	// falling
	for (int i = 0; i < 7; i++)
		fall.PushBack({ 1 + sprite_distance.x * i, 1 + sprite_distance.y * row, 547, 481 });

	fall.speed = 0.2;
	fall.loop = false;

	// wall slide right

	wallslideright.PushBack({ 1 + sprite_distance.x * 8, 1 + sprite_distance.y * row, 547, 481 });
	wallslideright.PushBack({ 1 + sprite_distance.x * 9, 1 + sprite_distance.y * row, 547, 481 });

	wallslideright.speed = 0.1;
}

void Player::ManagePhysics(float dt)
{
	App->collision->ManageGroundCollisions((GroundEntity*)this, dt);
}

void Player::OnCollision(Collider* collider)
{
	if (collider->type == COLLIDER_BONE)
	{
		if(!App->transition->transitioning)
		{ 
			if (App->map->map == 0)
			{
				points += 200;
				lvl2points = points;
				lvl2coins = coins;
				App->transition->Make_transition(&want_transition_1);
			}
			else if (App->map->map == 1)
			{
				points = points * (float)(1 + (float)1 / (float)lives);
				if (App->scene->m == 0 && App->scene->m2 == 0)
					points += 200;
				else if (App->scene->m == 1)
					points += 150;
				else if (App->scene->m == 2)
					points += 100;
				else if (App->scene->m == 3)
					points += 50;

				if (App->max_punctuation < points)
					App->max_punctuation = points;

				App->transition->Make_transition(&App->scene->want_load_credits, &App->scene->want_unload_HUD, &App->scene->want_unload_map);
			}
		}
	}
	else if ((collider->type == COLLIDER_DEADLY || collider->type == COLLIDER_ENEMY_GROUND) && !dead && this->collider->type != COLLIDER_GOD)
	{		
		App->audio->PlayFx(5);
		lives--;
		dead = true;
	}
}

void Player::Save(pugi::xml_node& data)
{
	data.append_attribute("x") = position.x;
	data.append_attribute("y") = position.y;
	data.append_attribute("map") = App->map->map;
	data.append_attribute("coins") = coins;
	data.append_attribute("points") = points;
	data.append_attribute("lvl2coins") = lvl2coins;
	data.append_attribute("lvl2points") = lvl2points;
	data.append_attribute("lives") = lives;
	data.append_attribute("max_punctuation") = App->max_punctuation;
}

void Player::Load(pugi::xml_node& data)
{
	position.x = data.attribute("x").as_float();
	position.y = data.attribute("y").as_float() - gravity * 2;
	coins = data.attribute("coins").as_int();
	points = data.attribute("points").as_int();
	lvl2coins = data.attribute("lvl2coins").as_int();
	lvl2points = data.attribute("lvl2points").as_int();
	lives = data.attribute("lives").as_int();
	App->max_punctuation = data.attribute("max_punctuation").as_int();

	if (App->map->map == 1)
	{
		coins = lvl2coins;
		points = lvl2points;
	}
}

void Player::Pause()
{
	if (animation != &fall && animation != &jump)
		animation->Reset();

	jump_timer.Pause();
	walljump_timer.Pause();
	slide_timer.Pause();
}

void Player::Resume()
{
	jump_timer.StartAfterPause();
	jump_timer.ResetPause();
	walljump_timer.StartAfterPause();
	walljump_timer.ResetPause();
	slide_timer.StartAfterPause();
	slide_timer.ResetPause();
}