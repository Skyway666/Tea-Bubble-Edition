#include "Button.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "ClickManager.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1Map.h"
#include "Pathfinding.h"
#include "j1Entities.h"
#include "Pathfinding.h"
#include "j1Audio.h"



Button::Button(int x, int y, j1Module* _listener, BUTTON_TYPE _button_type): Ui_element(x, y)
{
	type = BUTTON;

	switch (_button_type) {
		case STATE_CHANGE:
		{
			Idle.PushBack({ 5,237,277,103 });
			Shiny.PushBack({ 5,120,277,103 });
			Pressed.PushBack({ 5,6,277,103 });
			collider = App->gui->click_manager->Add_ui_collider({ x,y,277,95 });
			break;
		}
		case UPPER:
		{
			Idle.PushBack({320,19,121,75});
			Shiny.PushBack({ 320,101,121,75});
			Pressed.PushBack({320,19,121,75});
			collider = App->gui->click_manager->Add_ui_collider({ x,y,121,75 });
			break;
		}
		case LOWER:
		{
			Idle.PushBack({ 320,279,121,75 });
			Shiny.PushBack({ 320,201,121,75 });
			Pressed.PushBack({ 320,279,121,75 });
			collider = App->gui->click_manager->Add_ui_collider({ x,y,121,75 });
			break;
		}
	}


	Pressed.loop = false;
	Shiny.loop = false;
	Idle.loop = false;
	listener = _listener;
	

	animation = &Idle;
}


Button::~Button()
{

}

void Button::OnMouseEvent(UI_EVENT event)
{
	if (event == MOUSE_ENTER)
	{
		animation = &Shiny;
	}
	if (event == MOUSE_EXIT)
	{
		animation = &Idle;
	}
	if (event == MOUSE_CLICK)
	{
		animation = &Pressed;
	}
	if (event == MOUSE_STOP_CLICK)
	{
		animation = &Shiny;
	}
}
