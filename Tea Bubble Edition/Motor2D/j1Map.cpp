#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collisions.h"
#include <math.h>
#include "j1Entities.h"
#include "Player.h"

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());


	bone_animation.PushBack({ 0, 0, 212, 405 });
	bone_animation.PushBack({ 230, 0, 212, 405 });
	bone_animation.PushBack({ 450, 0, 212, 405 });

	bone_animation.speed = 0.2;

	return ret;
}

void j1Map::CreateCollidersAndEnemies()
{
	int counter = 0;
	int coin_counter = 0;
	

		uint height = 0; // Used to calculate wall tile height

		while (counter < data.layer_array.At(1)->data->height * data.layer_array.At(1)->data->width)
		{
			int id = data.layer_array.At(1)->data->data[counter];
			int x = counter; 
			int y = data.layer_array.At(1)->data->width;
			Get_coordinates_from_id(&x, &y);

			//Now, x and y are the coordinates of the tileset
			
			MapToWorld(&x, &y);

			uint lenght = 1; // Used to calculate pit lenght
			uint column_height = 1; // Used to calculate total wall height

			if (counter % data.width == 0)
				height++;

			//Now they are in pixels
			if (id == 11)
			{ 
				if (data.layer_array.At(1)->data->data != nullptr)
				{
						for (uint i = 1; y - data.tile_height * i > 0 && i < data.height; i++)
						{
							int c = counter - data.width * i;

							if (c > 0)
							{
								if (data.layer_array.At(1)->data->data[counter - data.width * i] == 11)
									column_height++;
								else
									break;
							}
						}
						for (uint i = 1; y + data.tile_height * i < data.tile_height * data.height && i < data.height; i++)
						{
							int c = counter + data.width * i;
							if (c < (data.width * data.height))
							{
								if (data.layer_array.At(1)->data->data[counter + data.width * i] == 11)
									column_height++;
								else
									break;
							}
						}

				}

				App->collision->AddCollider({ x, y, data.tilesets.At(0)->data->tile_width, data.tilesets.At(0)->data->tile_height }, COLLIDER_WALL, (j1Module*)nullptr, 1, height, column_height);

				if (data.layer_array.At(1)->data->data != nullptr && y != 0 && data.layer_array.At(1)->data->data[counter - data.width] != 11) // Set Walkable areas
					App->collision->AddCollider({ x, y - data.tilesets.At(0)->data->tile_height, data.tilesets.At(0)->data->tile_width, data.tilesets.At(0)->data->tile_height }, COLLIDER_WALKABLE);
			}
			else if (id == 12)
			{
				App->collision->AddCollider({ x, y, data.tilesets.At(0)->data->tile_width, data.tilesets.At(0)->data->tile_height }, COLLIDER_DEADLY);
			}
			else if (id == 23)
			{
				for (uint i = 1; data.layer_array.At(1)->data->data[counter - i] == 23; i++)
					lenght++;
				for (uint i = 1; data.layer_array.At(1)->data->data[counter + i] == 23; i++)
					lenght++;
				
				App->collision->AddCollider({ x, y, data.tilesets.At(0)->data->tile_width, data.tilesets.At(0)->data->tile_height }, COLLIDER_PIT, (j1Module*)nullptr, lenght);
			}
			else if (id == 19)
			{
				App->entities->Add_waiting_entity(ENTITY_TYPES::AIR_ENEMY, x, y);
			}
			else if (id == 21)
			{
				App->entities->Add_waiting_entity(ENTITY_TYPES::GROUND_ENEMY, x, y);
			}
			else if (id == 20)
			{
				coin_counter++;
				App->entities->Add_waiting_entity(ENTITY_TYPES::COIN, x, y - data.tile_height);

			}
			counter++;
		}

		App->collision->AddCollider({ data.bone_position.x, data.bone_position.y,bone_animation.GetCurrentFrame().w, bone_animation.GetCurrentFrame().h }, COLLIDER_BONE);
}

void j1Map::Draw()
{
	if (map_loaded == false)
		return;

	Player* player = (Player*)App->entities->player;
	//Blit background
     App->render->Blit(data.background_image, data.background_offset.x - player->player_x_displacement * data.parallax_speed, data.background_offset.y);


	//Blit bone
	 App->render->Blit(bone_graphics, data.bone_position.x, data.bone_position.y, 1 ,true, &bone_animation.GetCurrentFrame());


	int counter = 0;
		while (counter < data.layer_array.At(0)->data->height * data.layer_array.At(0)->data->width)
		{
			int id = data.layer_array.At(0)->data->data[counter]; //devuelve el tipo de tileset

			if(id != 0)
			{ 
				int x = counter;
				int y = data.layer_array.At(0)->data->width;
				Get_coordinates_from_id(&x, &y);

				//Now, x and y are the coordinates of the tileset

				MapToWorld(&x, &y);

				//Now they are in pixels
		
				App->render->Blit(data.tilesets.At(0)->data->texture, x, y, 1,true, &Tile_Rect(id));
			}
			counter++;
		}
}


void j1Map::MapToWorld(int* x, int* y) const
{
	*x = *x * data.tilesets.At(0)->data->tile_height;

	*y = *y * data.tilesets.At(0)->data->tile_width;
}

void j1Map::WorldToMap(int* x, int* y) const
{
	*x = *x /data.tilesets.At(0)->data->tile_height;

	*y = *y /data.tilesets.At(0)->data->tile_width;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id -  num_tiles_width * (relative_id / num_tiles_width)));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

SDL_Rect j1Map::Tile_Rect(int tileid)
{

	tileid = tileid - data.tilesets.At(0)->data->firstgid;

	int row = tileid / data.tilesets.At(0)->data->num_tiles_width;

	int column = tileid - row*data.tilesets.At(0)->data->num_tiles_width;

	SDL_Rect rect;
	rect.w = data.tilesets.At(0)->data->tile_width;
	rect.h = data.tilesets.At(0)->data->tile_height;

	rect.x = data.tilesets.At(0)->data->margin + (rect.w + data.tilesets.At(0)->data->spacing)*column;

	rect.y = data.tilesets.At(0)->data->margin + (rect.h + data.tilesets.At(0)->data->spacing)*row;

	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		App->tex->UnLoad(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();


	p2List_item<MapLayer*>* item_;
	item_ = data.layer_array.start;
	while (item != NULL)
	{
		RELEASE(item_->data);
		item_ = item_->next;
	}
	App->tex->UnLoad(data.background_image);
	data.layer_array.clear();
	map_file.reset();

	map_loaded = false;
	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;

	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}
	LoadBackground(map_file);
	LoadMapPropierties(map_file);

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;

	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}
	// Load layer info ----------------------------------------------
	LoadLayer(map_file);

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}
		
		p2List_item<MapLayer*>* item_layer = data.layer_array.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	if (ret = true)
	{
		CreateCollidersAndEnemies();
	}

	

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node)
{
	pugi::xml_node layer = node.child("map").child("layer");
	while (layer != nullptr)
	{
	    MapLayer* layer_data = new MapLayer;

		layer_data->height = layer.attribute("height").as_int();
		layer_data->width = layer.attribute("width").as_int();
		layer_data->name = layer.attribute("name").as_string();

		layer_data->data = new uint[layer_data->height*layer_data->width];

		memset(layer_data->data, 0, sizeof(uint)*(layer_data->height*layer_data->width));

		int i = 0;
		pugi::xml_node tile_ = layer.child("data").child("tile");
		while (tile_ != nullptr)
		{
			layer_data->data[i] = tile_.attribute("gid").as_uint();
			tile_ = tile_.next_sibling("tile");
			i++;
		}

		data.layer_array.add(layer_data);
		layer = layer.next_sibling("layer");
	}

	return true;
}

bool j1Map::LoadBackground(pugi::xml_node& node)
{
	data.background_image = App->tex->Load(PATH(folder.GetString(),node.child("map").child("imagelayer").child("image").attribute("source").as_string()));

	data.background_offset.x = node.child("map").child("imagelayer").attribute("offsetx").as_float();

	data.background_offset.y = node.child("map").child("imagelayer").attribute("offsety").as_float();

	return true;
}
bool j1Map::LoadMapPropierties(pugi::xml_node& node)
{
	pugi::xml_node iterator = node.child("map").child("properties").child("property");
	while (iterator != nullptr)
	{ 
		p2SString name = iterator.attribute("name").as_string();

		if(name == "parallax_speed")
		{ 
			data.parallax_speed = iterator.attribute("value").as_float();
		}

		if (name == "player_starting_valuex")
		{
			data.player_starting_value.x = iterator.attribute("value").as_float();
		}

		if (name == "player_starting_valuey")
		{
			data.player_starting_value.y = iterator.attribute("value").as_float();
		}

		if( name == "camera_y_limit")
		{ 
		    data.camera_y_limit = iterator.attribute("value").as_float();
		}

		if (name == "bone_positionx")
		{
			data.bone_position.x = iterator.attribute("value").as_float();
		}

		if (name == "bone_positiony")
		{
			data.bone_position.y = iterator.attribute("value").as_int();
		}

		iterator = iterator.next_sibling();
	}

	return true;
}

void j1Map::Get_coordinates_from_id(int* x, int* y)
{
	int row = *x / *y;
	int column = *x - row*(*y);

	*x = column;
	*y = row;
}

int j1Map::Get_id_from_coordinates(int x, int y)
{
	return (y*data.width) + x;
}



