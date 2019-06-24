#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Gui.h"

struct SDL_Texture;
class CupDispenser;

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

	bool exit_app = false;

private:

	_TTF_Font* title = nullptr;
	_TTF_Font* text_body = nullptr;
	// Game
	CupDispenser* cup_dispenser = nullptr;

	// UI
	Window* kawaii_cup = nullptr;


};

#endif // __j1SCENE_H__