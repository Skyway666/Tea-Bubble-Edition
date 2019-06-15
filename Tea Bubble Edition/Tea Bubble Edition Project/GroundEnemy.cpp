#include "GroundEnemy.h"



GroundEnemy::GroundEnemy(int x, int y, Ground_Enemy_Initial_Inf initial_inf) : GroundEntity(x, y)
{
	type = ENTITY_TYPES::GROUND_ENEMY;
	scale = 0.12;

	position.x = x;
	position.y = y;

	int row = 0;

	// running
	for (int i = 0; i < 8; i++)
		run.PushBack({ 579 * i, 1732 + 763 * row, 547, 763 });

	row++;

	// jumping
	for (int i = 0; i < 10; i++)
		jump.PushBack({ 579 * i, 1732 + 763 * row, 579, 763 });

	jump.loop = false;

	gravity = initial_inf.gravity;
	initial_jump_time = initial_inf.jump_time / 100;
	jump_time = initial_inf.jump_time / 100;
	speed_modifier.y = initial_inf.speed_modifier.y;
	speed_modifier.x = initial_inf.speed_modifier.x;

	speed.y = speed_modifier.y;
	speed.x = speed_modifier.x;

	SDL_Rect r{ 0, 0, 579, 763 };
	SDL_Rect collider_rect{ 0, 0, r.w * scale, r.h * scale };

	collider = App->collision->AddCollider(collider_rect, COLLIDER_ENEMY_GROUND, App->entities);
	animation = &run;
}


GroundEnemy::~GroundEnemy()
{

}

void GroundEnemy::Update(float dt, bool do_logic)
{
	if (contact.y == 1)
		spawned = true;

	frames++;
	
	animation = &run;
	jump.speed = 0.4 * dt;
	run.speed = 0.4 * dt;

	if (contact.y == 1)
	{
		if (position.x - App->entities->player->position.x >= 0)
			player_pos = -1;
		else
			player_pos = 1;
	}

	if (speed.x > 0)
		speed.x = speed_modifier.x;
	else if (speed.x < 0)
		speed.x = -speed_modifier.x;

	if (last_contact_y != 1 && contact.y == 1)
		just_landed = true;

	if (contact.y == 1 && contact.x != 0)
		turn = true;

	if (is_idle)
		Exec_idle();
	else if (!is_idle)
		Exec_attack();

	if (front_of_unwalkable && !is_idle)
		speed.x = 0;

	if (!jumping && contact.y == 1)
		jump_x = 0;

	if (jump_x != 0)
	{
		if (speed.x > 0)
			speed.x = jump_x;
		else
			speed.x = -jump_x;
	}

	if (!is_idle)
	{
		if (speed.x > 0 && player_pos == -1)
			speed.x = -speed.x;
		else if (speed.x < 0 && player_pos == 1)
			speed.x = -speed.x;
	}

	if (spawned)
		position.x += speed.x * dt;

	turn = false;

	Jump(dt);

	// Simulate gravity
	if (contact.y != 1)
		position.y += gravity * dt;

	// Make collider follow enemy
	if(collider != nullptr)
	collider->SetPos(position.x, position.y);

	last_contact_y = contact.y;
	contact.x = 0;
	contact.y = 0;
	front_of_unwalkable = false;
}

void GroundEnemy::Exec_idle()
{
	if (turn || front_of_unwalkable)
		speed.x = -speed.x;

	if (front_of_unwalkable && speed.x == 0)
	{
		if (player_pos == 1)
			speed.x = -speed_modifier.x;
		else
			speed.x = speed_modifier.x;
	}

	if (speed.x < 0)
		flip = true;
	else if (speed.x > 0)
		flip = false;

	front_of_unwalkable = false;
}

void GroundEnemy::Exec_attack()
{
	if (player_pos == -1)
		flip = true;
	else if (player_pos == 1)
		flip = false;
}

void GroundEnemy::Jump(float dt)
{
	// jump
	if (jumping && (!just_landed || jumping_wall))
	{
		if (jump_timer.IsOver() && contact.y == 1)
		{
			jump_timer.Start(jump_time);
			contact.y = 0;
			jump.Reset();
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
	else
		jumping = false;
}

void GroundEnemy::OnCollision(Collider* collider)
{
	if (collider->type == COLLIDER_PATH)
		is_idle = false;
	else if (collider->type == COLLIDER_WALKABLE)
		is_idle = true;
}

void GroundEnemy::ManagePhysics(float dt)
{
	App->collision->ManageGroundCollisions((GroundEntity*)this, dt);
	App->collision->EnemyJump(this, dt);
}
void GroundEnemy::Save(pugi::xml_node& data)
{
	data.append_attribute("x") = position.x;
	data.append_attribute("y") = position.y - gravity * 3;
	data.append_attribute("speed_x") = speed.x;
	data.append_attribute("contact_x") = contact.x;
	data.append_attribute("contact_y") = contact.y;
	data.append_attribute("player_pos") = player_pos;
	data.append_attribute("spawned") = spawned;
	data.append_attribute("is_idle") = is_idle;
	data.append_attribute("front_of_unwalkable") = front_of_unwalkable;
	data.append_attribute("frames") = frames;
	data.append_attribute("time") = time;
	data.append_attribute("just_landed") = just_landed;
	data.append_attribute("jumping_wall") = jumping_wall;
	data.append_attribute("jump_x") = jump_x;
	data.append_attribute("allow_time") = allowtime;
	data.append_attribute("jumping") = jumping;

	jump_timer.Save();
}

void GroundEnemy::Load(pugi::xml_node& data)
{
	position.x = data.attribute("x").as_float();
	position.y = data.attribute("y").as_float();
	speed.x = data.attribute("speed_x").as_float();
	contact.x = data.attribute("contact_x").as_int();
	contact.y = data.attribute("contact_y").as_int();
	player_pos = data.attribute("player_pos").as_int();
	is_idle = data.attribute("is_idle").as_bool();
	front_of_unwalkable = data.attribute("front_of_unwalkable").as_bool();
	frames = data.attribute("frames").as_int();
	time = data.attribute("time").as_int();
	just_landed = data.attribute("just_landed").as_bool();
	jumping_wall = data.attribute("jumping_wall").as_bool();
	jump_x = data.attribute("jump_x").as_float();
	allowtime = data.attribute("allowtime").as_bool();
	jumping = data.attribute("jumping").as_bool();

	if (jumping)
		spawned = true;
	else
		spawned = false;

	jump_timer.Load();
}

void GroundEnemy::Pause()
{
	animation->Reset();
	jump_timer.Pause();
}

void GroundEnemy::Resume()
{
	jump_timer.StartAfterPause();
	jump_timer.ResetPause();
}