#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "Icon.h"
#include "Button.h"
#include "Text.h"
#include "Window.h"
#include "StatBar.h"
#include "p2List.h"


#define CURSOR_WIDTH 2

class ClickManager;
struct Ui_collider;
enum UI_ELEMENT_TYPE //Type of ui elements avaliable
{
	ICON,
	BUTTON,
	TEXT,
	WINDOW,
	STATBAR
};
enum BUTTON_TYPE //Knowing what type of button we are creating, just for animation purpuses
{
	NONE,
	STATE_CHANGE,
	UPPER,
	LOWER
};
enum UI_EVENT
{
	MOUSE_ENTER,
	MOUSE_EXIT,
	MOUSE_CLICK,
	MOUSE_STOP_CLICK,
};
// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Update of all ui_elements will be executed here
	bool PreUpdate();

	// Draw of all ui_elements and background will be executed here
	bool PostUpdate();

	// Update icon animations' speed
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	//Add icon
	Icon* Add_icon(int x, int y);
	//Add button
	Button* Add_button(int x, int y, j1Module* _listener, BUTTON_TYPE button_type = STATE_CHANGE);
	//Add text as UI
	Text* Add_text(int x, int y, const char* text, SDL_Color color = {0,0,0,255}, _TTF_Font* font = nullptr);
	//Add window
	Window* Add_window(int x, int y);
	//Add StatBar
	StatBar* Add_StatBar(int x, int y, int w, int h, float* variable, float variable_starting_value);
	//This function will look for the Ui_element that has to be deleted among all the lists and delete it. It will return "false" if not found it, and true if found.
	//It will also erase all childs
	bool Erase_Ui_element(Ui_element* element);

	//This method will iterate over all the colliders of the icons in the "ui_elements" list, looking for the one that has the same collider that the 
	//one given to the function. Then it will call the global "OnClick" method and "OnClick" method of the UI element, which will change the label.
	void OnMouseEvent_caller(Ui_collider* c1, UI_EVENT event);

	//Sets a background out of the passed texture
	void Set_backgrond(SDL_Texture* texture);

	const SDL_Texture* GetAtlas() const;
	const SDL_Texture* GetHUD() const;
	
	ClickManager* click_manager;
private:

	SDL_Texture* atlas;
	SDL_Texture* atlas2;
	SDL_Texture* HUD;
	//Background image
	SDL_Texture* current_background; 

	//Paths to load assets
	p2SString atlas_file_name;
	p2SString atlas2_file_name;
	p2SString HUD_file_name;

	
	//Icons list
	p2List<Icon*> icons;
	//Buttons list
	p2List<Button*> buttons;
	//Text list
	p2List<Text*> texts;
	//Windows list
	p2List<Window*> windows;
	//Statbars list
	p2List<StatBar*> statbars;

	//If true, it allows debug draw for ui
	bool debug_draw = false; 
	



};

#endif // __j1GUI_H__