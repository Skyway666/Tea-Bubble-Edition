#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "ClickManager.h"
//This structure contains a pointer to a text and its offset. Its only use is to be passed to the icon or button constructor in order to have a text
//linked to it


j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
	click_manager = new ClickManager();
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;


	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	test_texture = App->tex->Load("textures/Bubble Tea Prototipo_01.png");
	return true;
}

// Update of all ui_elements and click_manager will be executed here
bool j1Gui::PreUpdate()
{
	//Update all windows
	for (uint i = 0; i < windows.count(); ++i)
	{
		if (windows[i] != nullptr && windows[i]->active)
			windows[i]->Update();
	}
	//Update all icons 
	for (uint i = 0; i < icons.count(); ++i)
	{
		if (icons[i] != nullptr && icons[i]->active)
			icons[i]->Update();
	}
	//Update all buttons
	for (uint i = 0; i < buttons.count(); ++i)
	{
		if (buttons[i] != nullptr && buttons[i]->active)
			buttons[i]->Update();
	}
	//Update all statbars
	for (uint i = 0; i < statbars.count(); ++i)
	{
		if (statbars[i] != nullptr && statbars[i]->active)
			statbars[i]->Update();
	}
	//Update all texts
	for (uint i = 0; i < texts.count(); ++i)
	{
		if (texts[i] != nullptr && texts[i]->active)
			texts[i]->Update();
	}

	click_manager->Update();
	
	return true;
}

bool j1Gui::Update(float dt)
{
	// Update all icon animations' speed
	for (uint i = 0; i < icons.count(); ++i)
	{
		if (icons[i] != nullptr && icons[i]->animation != nullptr)
			icons[i]->animation->speed = icons[i]->animation_speed * dt;
	}

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		debug_draw = !debug_draw;
	}

	return true;
}

// Draw of all ui_elements and background will be executed here
bool j1Gui::PostUpdate()
{
	
		//Blit all windows
		for (uint i = 0; i < windows.count(); ++i)
		{
			if (windows[i] != nullptr && windows[i]->active)
				windows[i]->Draw(test_texture);
		}
		//Blit all icons (Maybe they should be able to blit from their own texture like texts)
		for (uint i = 0; i < icons.count(); ++i)
		{
			if (icons[i] != nullptr && icons[i]->active)
				icons[i]->Draw(test_texture, icons[i]->scale);
		}
		//Blit all buttons
		for (uint i = 0; i < buttons.count(); ++i)
		{
			if (buttons[i] != nullptr && buttons[i]->active)
				buttons[i]->Draw(test_texture);
		}
		//Blit all statbars
		for (uint i = 0; i < statbars.count(); ++i)
		{
			if (statbars[i] != nullptr && statbars[i]->active)
				statbars[i]->Draw();
		}
		//Blit all texts
		for (uint i = 0; i < texts.count(); ++i)
		{
			if (texts[i] != nullptr && texts[i]->active)
				texts[i]->Draw();
		}


	if(debug_draw)
	click_manager->DebugDraw();

	return true;
}

Icon* j1Gui::Add_icon(int x, int y)
{
	Icon* new_ui_element = nullptr;

	new_ui_element = new Icon(x, y);
	icons.add(new_ui_element);

	return new_ui_element;
}

Button* j1Gui::Add_button(int x, int y, j1Module* _listener, BUTTON_TYPE button_type)
{
	Button* new_ui_element = nullptr;

	new_ui_element = new Button(x, y,_listener,button_type);
	buttons.add(new_ui_element);

	return new_ui_element;
}

Text* j1Gui::Add_text(int x, int y, const char* text, SDL_Color color, _TTF_Font* font)
{
	Text* new_text = new Text(x, y, text,color, font);
	texts.add(new_text);

	return new_text;
}
Window* j1Gui::Add_window(int x, int y)
{
	Window* new_window = new Window(x, y);
	windows.add(new_window);

	return new_window;
}

StatBar* j1Gui::Add_StatBar(int x, int y, int w, int h, float* variable,float variable_starting_value)
{
	StatBar* new_statbar = new StatBar(x, y, w, h, variable, variable_starting_value);
	statbars.add(new_statbar);

	return new_statbar;
}

//This method will iterate over all the colliders of the buttons in the "buttons" list, looking for the one that has the same collider that the one given to the 
//function. Then it will call its "OnClick" method
void j1Gui::OnMouseEvent_caller(Ui_collider* c1, UI_EVENT event)
{
	for (int i = 0; i < buttons.count(); i++)
	{
		if(buttons[i] != nullptr)
		{
			if (buttons[i]->collider == c1)
			{
				buttons[i]->listener->OnMouseEvent(event, buttons[i]);
				buttons[i]->OnMouseEvent(event);
			}
		}
	}
	for (int i = 0; i < windows.count(); i++)
	{
		if (windows[i] != nullptr)
		{
			if (windows[i]->collider == c1)
			{
				windows[i]->OnMouseEvent(event);
			}
		}
	}
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	for (int i = 0; i < icons.count(); i++)
	{
		delete icons[i];
		icons[i] = nullptr;
	}
	for (int i = 0; i < buttons.count(); i++)
	{
		delete buttons[i];
		buttons[i] = nullptr;
	}
	for (int i = 0; i < texts.count(); i++)
	{
		delete texts[i];
		texts[i] = nullptr;
	}
	for (int i = 0; i < windows.count(); i++)
	{
		delete windows[i];
		windows[i] = nullptr;
	}
	for (int i = 0; i < statbars.count(); ++i)
	{
		delete statbars[i];
		statbars[i] = nullptr;
	}
	click_manager->Cleanup();//Free all ui_colliders
	delete click_manager;
	click_manager = nullptr;
	return true;
}

bool j1Gui::Erase_Ui_element(Ui_element* element)
{
	if (element == nullptr)
		return false;

	switch (element->type) {
		case ICON:
		{
			int index = icons.find((Icon*)element);

			if (index == -1)
				return false;

			p2List_item<Icon*>* node_to_delete = icons.At(index);

			for (int i = 0; i < icons[index]->linked_elements.count(); i++)
			{
				Erase_Ui_element(icons[index]->linked_elements[i]);
			}
			
			delete icons[index];
			icons.del(node_to_delete);
			return true;
		}
		case BUTTON:
		{
			int index = buttons.find((Button*)element);

			if (index == -1)
				return false;

			p2List_item<Button*>* node_to_delete = buttons.At(index);

			for (int i = 0; i < buttons[index]->linked_elements.count(); i++)
			{
				Erase_Ui_element(buttons[index]->linked_elements[i]);
			}


			click_manager->Erase_ui_collider(buttons[index]->collider);

			delete buttons[index];
			buttons.del(node_to_delete);
			return true;
		}
		case TEXT:
		{
			int index = texts.find((Text*)element);

			if (index == -1)
				return false;

			p2List_item<Text*>* node_to_delete = texts.At(index);

			for (int i = 0; i < texts[index]->linked_elements.count(); i++)
			{
				Erase_Ui_element(texts[index]->linked_elements[i]);
			}

			delete texts[index];
			texts.del(node_to_delete);
			return true;
		}
		case WINDOW:
		{
			int index = windows.find((Window*)element);

			if (index == -1)
				return false;

			p2List_item<Window*>* node_to_delete = windows.At(index);

			for (int i = 0; i < windows[index]->linked_elements.count(); i++)
			{
				Erase_Ui_element(windows[index]->linked_elements[i]);
			}

			click_manager->Erase_ui_collider(windows[index]->collider);

			delete windows[index];
			windows.del(node_to_delete);
			return true;
		}
		case STATBAR:
		{
			int index = statbars.find((StatBar*)element);

			if (index == -1)
				return false;

			p2List_item<StatBar*>* node_to_delete = statbars.At(index);

			for (int i = 0; i < statbars[index]->linked_elements.count(); i++)
			{
				Erase_Ui_element(statbars[index]->linked_elements[i]);
			}

			click_manager->Erase_ui_collider(statbars[index]->collider);

			delete statbars[index];
			statbars.del(node_to_delete);
			return true;
		}
	}

	return false;
}
// class Gui ---------------------------------------------------

