#pragma once
#include "Ui_element.h"

class Icon : public Ui_element
{
public:
	Icon(int x, int y); //It should recive a pointer to the texture and animations should be set by the creator of the UI element
	~Icon();

	float animation_speed = 1;
	Animation anim;
	float scale = 1;
};

