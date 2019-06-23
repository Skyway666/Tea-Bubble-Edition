#pragma once
//This class will function very similarly to how "colliders" does. It will create and store all the "ui_colliders" in a list, and iterate them checking for 
//collisions in every frame. If a collider is clicked, the "OnClick" method of the j1Gui module will be called, and the collider will be passed as an argument.
//Then, the "OnCollision" method will compare that collider with all the colliders of the different UI elements (if they have one) and call the "OnCollision" method 
//of the one that has been clicked. Then, the appropiated code will be executed.

//The exact same procedure will function with "OverClick", which will trigger the animation of the mouse being pressed, and probably nothing else.

#include "j1Gui.h"

struct Ui_collider
{
	Ui_collider(SDL_Rect _rect)
	{
		rect = _rect;
		mouse_over = false;
		active = true;
	}
	bool CheckCollision(iPoint point)
	{
		return point.x > rect.x && point.x < rect.x + rect.w && point.y > rect.y && point.y < rect.y + rect.h;
	}
	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}
	SDL_Rect rect;
	bool mouse_over;
	bool active;
};

class ClickManager
{
public:
	ClickManager();
	~ClickManager();
	//Adds a collider to the collider list
	Ui_collider* Add_ui_collider(SDL_Rect _rect);
	//Erases a collider from the list, returns true if it was successfully removed and false if not
	bool Erase_ui_collider(Ui_collider* collider);
	//Will check all colliders compare it to the mouse position and click. Then if needed, will call the "OnClick" or "MouseOver" method of "j1Gui", passing
	//the indicated collider as an argument
	void Update();
	//Debug the colliders
	void DebugDraw();
	//Will empty the colliders list
	void Cleanup();

private:
	p2List<Ui_collider*> colliders;
};

