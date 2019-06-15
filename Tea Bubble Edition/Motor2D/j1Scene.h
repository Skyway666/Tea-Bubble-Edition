#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "Pathfinding.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Entities.h"
#include "j1Gui.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:
	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Executes code depending on the event and the ui element affected
	void OnMouseEvent(UI_EVENT event, Ui_element* element);
    
	//Unloads current map and loads the map indicated in the argument. If there is no map loaded, it just loads the indicated map
	void Change_to_map(int);
	//Unloads current map
	void Unload_map(); bool want_unload_map = false;

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	//Functions to load and unload Ui setups and bools to execute them safely
	void Load_main_menu(); bool want_load_main_menu = false; 
	void UnLoad_main_menu(); bool want_unload_main_menu = false; 
	void Load_credits(); bool want_load_credits = false;
	void UnLoad_credits(); bool want_unload_credits = false;
	void Load_pause(); bool want_load_pause = false;
	void UnLoad_pause(); bool want_unload_pause = false;
	void Load_options(); bool want_load_options = false;
	void UnLoad_options(); bool want_unload_options = false;
	void Load_HUD(); bool want_load_HUD = false;
	void UnLoad_HUD(); bool want_unload_HUD = false; // Also unloads player and plays menu music

	bool exit_app = false;
	bool coin_animation = false;

private:
	//Main menu setup

	Button* start = nullptr;
	Button* continuee = nullptr;
	Button* options = nullptr;
	Button* exit = nullptr;
	Button* credits = nullptr;
	SDL_Texture* main_menu_background;
	
	//Pause menu setup
	Window* Pause_Window = nullptr;
	Button* resume = nullptr;
	Button* exit_main_menu_fg = nullptr;
	bool pause_menu_spawned = true;
	
	//Credits setup
	Button* exit_main_menu_fc;
	Text*   main_title; //All the other text labels will be child of main title
	char max_points_text[10];
	
	//Options setup
	Button* exit_main_menu_fo = nullptr;
	Button* upper_music_volume = nullptr;
	Button* lower_music_volume = nullptr;
	Button* upper_fx_volume = nullptr;
	Button* lower_fx_volume = nullptr;
	StatBar* music_volume = nullptr;
	StatBar* fx_volume = nullptr;

	// HUD setup
	Icon* hourglass = nullptr;
	Icon* coins = nullptr;
	Icon* lives = nullptr;
	Icon* pointsicon = nullptr;
	char currentcoins[10];
	char currentpoints[10];
	char time[10];
	Text* hourglass_time;
	Text* current_coins;
	Text* current_points;
	void UpdateTime();
	void ResetPlaytime();
	int LivesFrame = 0;

	//Paths to load assets from config
	p2SString main_menu_background_file_name;

	_TTF_Font* title = nullptr;
	_TTF_Font* text_body = nullptr;
	bool continuing = false;

public:
	// Seconds, minutes and hours (should not be public, but their load and save is currently being managed in player)
	int s = 0;
	int s2 = 0;
	int m = 0;
	int m2 = 0;
	int h = 0;
	int h2 = 0;
	j1Timer playtime;

};

#endif // __j1SCENE_H__