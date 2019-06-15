#pragma once
#include "Ui_element.h"
class Window : public Ui_element
{
public:
	Window(int x, int y);
	~Window();

	//Detect mouse events on ui element internally
	void OnMouseEvent(UI_EVENT event);
	//Be able to move window and update child objects
	void Update();



private:

	Animation Idle;
	//Variables to be able to move UI elements
	bool gripped = false; //Knowing if the variable is gripped
	bool mouse_pos_gotten = false; //Bool to know if the mouse position has been taken
	iPoint mouse_pos_when_grip; //Knowing the initial position of the mouse when the ui element has been gripped
	iPoint pos_when_grip; //Position when grip
	iPoint offset_when_grip; //Offset when grip
};

