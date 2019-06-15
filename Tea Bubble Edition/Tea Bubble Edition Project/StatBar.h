#pragma once
#include "Icon.h"
class StatBar : public Ui_element
{
public:
	StatBar(int x, int y, int w, int h, float* _variable, float _variable_starting_value);
	~StatBar();
	void Update(); //Update the bar depending on the value of the variable
	void Draw(); //Draw the bar
private:
	SDL_Rect bar; //Bar that represents the length of a variable
	float* variable; //Variable represented by the bar
	float variable_starting_value;
	int bar_width_starting_value;
	
};

