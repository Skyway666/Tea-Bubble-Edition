#include "Ui_element.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "ClickManager.h"
#include "j1Input.h"




Ui_element::Ui_element(int x, int y)
{
	position.x = x;
	position.y = y;
}


Ui_element::~Ui_element()
{

}

void Ui_element::Draw(SDL_Texture* sprite, float scale)
{
	if(collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (animation != nullptr)
	{
		App->render->Blit(sprite, position.x, position.y, scale, false, &(animation->GetCurrentFrame()));
	}
}

void Ui_element::Link_ui_element(Ui_element* element, int offset_x, int offset_y)
{
	element->position.x = position.x + offset_x;
	element->position.y = position.y + offset_y;
	element->offset.x = offset_x;
	element->offset.y = offset_y;

	linked_elements.add(element);
}

void Ui_element::Update()
{
	//Update position of all linked objects to match parent
	for (int i = 0; i < linked_elements.count(); i++)
	{
		linked_elements[i]->position = position + linked_elements[i]->offset;
	}
}

void Ui_element::SetActive(bool activate)
{
	if (activate)
	{
		active = true;
		if (collider != nullptr)
		collider->active = true;
		for (int i = 0; i < linked_elements.count(); i++)
		{
			linked_elements[i]->SetActive(true);

		}

	}
	else
	{
		active = false;
		if (collider != nullptr)
		collider->active = false;
		for (int i = 0; i < linked_elements.count(); i++)
		{
			linked_elements[i]->SetActive(false);
 		}
	}
}