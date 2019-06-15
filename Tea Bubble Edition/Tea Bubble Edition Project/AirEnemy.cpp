#include "AirEnemy.h"
#include "Brofiler/Brofiler.h"



AirEnemy::AirEnemy(int x, int y, Flying_Enemy_Initial_Inf initial_inf): Entity(x, y)
{
	scale = 0.07;
	type = ENTITY_TYPES::AIR_ENEMY;

	idle.PushBack({ 0,0,1135,845 });
	idle.PushBack({ 1135,0,1135,845 });
	idle.PushBack({ 1135 * 2,0,1135,845 });
	idle.PushBack({ 1135 * 3,0,1135,845 });
	idle.PushBack({ 0,845,1135,845 });
	idle.PushBack({ 1135,845,1135,845 });
	idle.PushBack({ 1135 * 2,845,1135,845 });
	idle.PushBack({ 1135 * 3,845,1135,845 });


	iPoint scaledw_h(1135 * scale, 845 * scale);
	collider = App->collision->AddCollider({x, y, scaledw_h.x,scaledw_h.y}, COLLIDER_DEADLY, App->entities);
	idle.loop = true;

	side_fly_time = initial_inf.side_fly_time; 
	agro_distance = initial_inf.agro_distance; 
	tired_distance = initial_inf.tired_distance; 
	idle_speed = -initial_inf.idle_speed; 
	speed_modifier.x = initial_inf.speed_modifier.x; 
	speed_modifier.y = initial_inf.speed_modifier.y;


	is_idle = true;
	next_tile = 0;
	initial_tile = iPoint(x, y); 
	App->map->WorldToMap(&initial_tile.x, &initial_tile.y);	
    returning = false;
	home_path_found = false;
	speed.x = 0;
	speed.y = 0;
	animation = &idle;
}


AirEnemy::~AirEnemy()
{

}

void AirEnemy::Update(float dt, bool do_logic)
{
	//Maybe should be a function
	center.x = position.x + (1135 * scale)/2;
	center.y = position.y + (845 * scale)/2;

	idle.speed = 0.3 * dt;
	if (is_idle)
		Exec_idle();
	else
		Exec_path();

	if (distance_to_player() < agro_distance && !returning)
	{
		is_idle = false;
	}
	if(distance_to_start() > tired_distance)
	{
     	returning = true;
	}

    if (do_logic)
	{ 
		BROFILER_CATEGORY("Pathfinding", 0xFFDB7093);
		if (!returning && !is_idle)
		{
			Find_path_player();
		}
		else if (returning)
		{
			if(!home_path_found)
			{ 
				Find_path_home();
				home_path_found = true;
			}
			iPoint monster_map_pos(center.x, center.y);
			App->map->WorldToMap(&monster_map_pos.x, &monster_map_pos.y);
			if (monster_map_pos == initial_tile)
			{
				returning = false;
				is_idle = true;
				if (idle_speed > 0)
				{
					idle_speed = -idle_speed;
				}
				home_path_found = false;
			}
		}
	}


	if (speed.x < 0)
		flip = true;
	else if(speed.x > 0)
		flip = false;

	//Add speed to position
	position.x += speed.x * dt;
	position.y += speed.y * dt;

	//Make collider follow enemy
	collider->SetPos(position.x, position.y);   
}
void AirEnemy::Exec_idle()
{
	chase_timer.Reset();

	if (side_fly_timer.IsOver())
	{
		idle_speed = -idle_speed;
		side_fly_timer.Start(side_fly_time);
	}

	speed.x = idle_speed;
	speed.y = 0;
}

void AirEnemy::Exec_path()
{
	if (chase_timer.IsOver())
	{
		App->entities->player->points += 5;
		chase_timer.Start(2);
	}
	iPoint monster_map_pos(center.x, center.y);
	App->map->WorldToMap(&monster_map_pos.x, &monster_map_pos.y);

	if (path_to_follow.At(next_tile) != nullptr)
	{ 
		if (*path_to_follow.At(next_tile) == monster_map_pos)
		{
			next_tile++;
		}
		else
		{
			iPoint tile_to_reach = *path_to_follow.At(next_tile);

			speed.y = (tile_to_reach.y - monster_map_pos.y)*speed_modifier.y;
			speed.x = (tile_to_reach.x - monster_map_pos.x)*speed_modifier.x;
		}
	}
	else
	{
		speed.x = 0;
		speed.y = 0;
	}

}

void AirEnemy::Find_path_player()
{
	//Pathfinding
	iPoint player_map_pos(App->entities->player->center.x, App->entities->player->center.y);
	iPoint monster_map_pos(center.x, center.y);

	App->map->WorldToMap(&player_map_pos.x, &player_map_pos.y);
	App->map->WorldToMap(&monster_map_pos.x, &monster_map_pos.y);

	App->pathfinding->CreatePath(monster_map_pos, player_map_pos);

	path_to_follow.Clear();
	
	for (int i = 0; i < App->pathfinding->GetLastPath()->Count(); i++)
	{
		path_to_follow.PushBack(*App->pathfinding->GetLastPath()->At(i));
	}
	next_tile = 0;
	if(App->collision->debug == true)
	App->pathfinding->DebugDraw();
}

void AirEnemy::Find_path_home()
{
	BROFILER_CATEGORY("Backtracking", 0xFFFFEFD5);
	//Pathfinding
	iPoint monster_map_pos(center.x, center.y);

	App->map->WorldToMap(&monster_map_pos.x, &monster_map_pos.y);

	App->pathfinding->CreatePath(monster_map_pos, initial_tile);

	path_to_follow.Clear();

	for (int i = 0; i < App->pathfinding->GetLastPath()->Count(); i++)
	{
		path_to_follow.PushBack(*App->pathfinding->GetLastPath()->At(i));
	}
	next_tile = 0;

	if (App->collision->debug == true)
		App->pathfinding->DebugDraw();
}


void AirEnemy::OnCollision() 
{
	
}

int AirEnemy::distance_to_player()
{
	iPoint monster_map_pos(center.x, center.y);
	App->map->WorldToMap(&monster_map_pos.x, &monster_map_pos.y);
	iPoint player_map_pos(App->entities->player->center.x, App->entities->player->center.y);
	App->map->WorldToMap(&player_map_pos.x, &player_map_pos.y);

	int ret = sqrt(pow(player_map_pos.x - monster_map_pos.x, 2) + pow(player_map_pos.y - monster_map_pos.y, 2));
	return ret;
}

int AirEnemy::distance_to_start()
{
	iPoint monster_map_pos(center.x, center.y);
	App->map->WorldToMap(&monster_map_pos.x, &monster_map_pos.y);

	int ret = sqrt(pow(initial_tile.x - monster_map_pos.x, 2) + pow(initial_tile.y - monster_map_pos.y, 2));
	return ret;
}
void AirEnemy::Save(pugi::xml_node& data)
{
	data.append_attribute("x") = position.x;
	data.append_attribute("y") = position.y;
	data.append_attribute("returning") = returning;
	data.append_attribute("is_idle") = is_idle;
	data.append_attribute("idle_speed") = idle_speed;

	side_fly_timer.Save();
}

void AirEnemy::Load(pugi::xml_node& data)
{
	position.x = data.attribute("x").as_int();
	position.y = data.attribute("y").as_int();
	returning = data.attribute("returning").as_bool();
	is_idle = data.attribute("is_idle").as_bool();
	idle_speed = data.attribute("idle_speed").as_int();

	side_fly_timer.Load();
}

void AirEnemy::Pause()
{
	animation->Reset();
	side_fly_timer.Pause();
	chase_timer.Pause();
}

void AirEnemy::Resume()
{
	side_fly_timer.StartAfterPause();
	side_fly_timer.ResetPause();
	chase_timer.StartAfterPause();
	chase_timer.ResetPause();
}