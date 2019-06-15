#include "Pathfinding.h"


Pathfinding::Pathfinding()
{

}


Pathfinding::~Pathfinding()
{
}

void Pathfinding::CreatePath(iPoint origin, iPoint destination) 
{		
	last_path.Clear();
	frontier.Clear();
	visited.clear();
	breadcrumbs.clear();
	if(IsWalkable(origin) && IsWalkable(destination))
	{ 

		iPoint curr = origin;
		frontier.Push(curr, curr.DistanceManhattan(destination));
    
		//Expanding Loop
		for(int i = 0; i != width * height; i++)
		{
			frontier.Pop(curr);

			if (curr == destination) //Create path
			{
				Path_BackTracking(curr);
				break;
			}

				iPoint neighbors[8];
				neighbors[0].create(curr.x + 1, curr.y + 0);
				neighbors[1].create(curr.x + 1, curr.y - 1);
				neighbors[2].create(curr.x + 0, curr.y - 1);
				neighbors[3].create(curr.x - 1, curr.y - 1);
				neighbors[4].create(curr.x - 1, curr.y + 0);
				neighbors[5].create(curr.x - 1, curr.y + 1);
				neighbors[6].create(curr.x + 0, curr.y + 1);
				neighbors[7].create(curr.x + 1, curr.y + 1);

				for (uint i = 0; i < 8; ++i)
				{
					if (IsWalkable(neighbors[i]) && visited.find(neighbors[i]) == -1)
					{
						int new_cost = neighbors[i].DistanceTo(destination);
						frontier.Push(neighbors[i], new_cost);
						visited.add(neighbors[i]);
						breadcrumbs.add(curr);
					
					}
				}
			} 
	}
}

void Pathfinding::Path_BackTracking(iPoint goal)
{
	last_path.PushBack(goal);

	int last_value = visited.find(goal) + 1;
	for (int i = visited.find(goal); i < last_value && i >= 0; i = visited.find(breadcrumbs.At(i)->data))
	{

		last_path.PushBack(breadcrumbs.At(i)->data);


		if (i < last_value)
		{
			last_value = i;
		}
	}

	last_path.Flip();
}

void Pathfinding::SetMap()
{
	walkability_values = App->map->data.layer_array.At(1)->data->data;
	width = App->map->data.width;
	height = App->map->data.height;
}

bool Pathfinding::CleanUp()
{
	last_path.Clear();

	return true;
}

p2DynArray<iPoint>* Pathfinding::GetLastPath()
{
	return &last_path;
}

bool Pathfinding::IsWalkable(const iPoint& pos)const
{

	bool boundaries = (pos.x >= 0 && pos.x <= (int)width && pos.y >= 0 && pos.y <= (int)height);
	if (!boundaries)
		return false;

	bool walkable = (walkability_values[(pos.y * width) + pos.x] != 11 && walkability_values[(pos.y * width) + pos.x] != 12);
	if (!walkable) //Check boundaries
		return false;

	return true;
}

void Pathfinding::DebugDraw()
{
	for (int i = 0; i < last_path.Count(); i++)
	{
		iPoint pos(last_path.At(i)->x, last_path.At(i)->y);
		App->map->MapToWorld(&pos.x, &pos.y);
		App->render->Blit(App->map->path_indicator, pos.x, pos.y, 0.1);
	}
}