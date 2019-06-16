#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "Animation.h"
#include "j1Render.h"

struct TileSet
{

	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture = nullptr;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

struct MapLayer
{
	p2SString name;
	int width;
	int height;
	uint* data = nullptr;
};



enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*> layer_array;
	fPoint player_starting_value; 
	SDL_Texture* background_image = nullptr;
	float parallax_speed; 
	int camera_y_limit; 
	iPoint bone_position;
	fPoint background_offset;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	void MapToWorld(int* x, int* y) const;

	void WorldToMap(int* x, int* y) const;

	void Get_coordinates_from_id(int* x, int* y); //la x is id,  y how much elements are there in a row

	int Get_id_from_coordinates(int x, int y);

	SDL_Rect Tile_Rect(int tileid);
  
	int map = -1; //Starting map (-1 for no map, 0 for first, 1 for second)

	SDL_Texture* bone_graphics = nullptr;
	SDL_Texture* path_indicator = nullptr;



	Animation bone_animation;


private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node);
	bool LoadBackground(pugi::xml_node& node);
	bool LoadMapPropierties(pugi::xml_node& node);

public:

	MapData data;
	bool map_loaded;

private:

	pugi::xml_document	map_file;
	p2SString			folder;

};

#endif // __j1MAP_H__