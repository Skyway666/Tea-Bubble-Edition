#include "Window.h"
#include "j1App.h"
#include "j1Gui.h"
#include "ClickManager.h"
#include "j1Input.h"


Window::Window(int x, int y): Ui_element(x, y)
{
	Idle.PushBack({ 140,1522,449,483 });
	Idle.loop = false;

	collider = App->gui->click_manager->Add_ui_collider({ x,y,449,100 });
	animation = &Idle;
	type = WINDOW;
}


Window::~Window()
{
}

void Window::Update()
{
	//Move object if gripped
	iPoint mouse_position;
	App->input->GetMousePosition(mouse_position.x, mouse_position.y);
	iPoint mouse_displacement;
	if (gripped && !mouse_pos_gotten)
	{
		mouse_pos_when_grip = mouse_position;
		pos_when_grip = position;
		offset_when_grip = offset;
		mouse_pos_gotten = true;
	}
	mouse_displacement = mouse_position - mouse_pos_when_grip;
	if (gripped)
	{
		position = pos_when_grip + mouse_displacement;
		offset = offset_when_grip + mouse_displacement;
	}
	if (!gripped && mouse_pos_gotten)
	{
		mouse_pos_gotten = false;
	}

	//Update position of all linked objects to match parent
	for (int i = 0; i < linked_elements.count(); i++)
	{
		linked_elements[i]->position = position + linked_elements[i]->offset;
	}

}

void Window::OnMouseEvent(UI_EVENT event)
{
	if (event == MOUSE_CLICK)
	{
		gripped = true;
	}
	if (event == MOUSE_STOP_CLICK)
	{
		gripped = false;
	}
}