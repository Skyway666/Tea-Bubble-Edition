#include "StatBar.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"



StatBar::StatBar(int x, int y, int w, int h, float* _variable, float _variable_starting_value) : Ui_element(x, y)
{
	bar = { x, y, w, h };
	variable = _variable;
	variable_starting_value = _variable_starting_value;
	bar_width_starting_value = w;

	type = STATBAR;
}


StatBar::~StatBar()
{
	
}

void StatBar::Update()
{
	//Update bar's position
	bar.x = position.x;
	bar.y = position.y;
	
	//Update bar's length
	bar.w = bar_width_starting_value * (*variable/variable_starting_value);

	//Update position of all linked objects to match parent
	for (int i = 0; i < linked_elements.count(); i++)
	{
		linked_elements[i]->position = position + linked_elements[i]->offset;
	}
}

void StatBar::Draw()
{
	App->render->DrawQuad(bar, 253, 212, 62, 255, true, false);
}