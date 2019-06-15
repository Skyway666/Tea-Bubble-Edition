#include "Text.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Render.h"
#include "p2SString.h"
#include "j1Gui.h"


Text::Text(int x, int y, const char* _string, SDL_Color _color, _TTF_Font* _font) : Ui_element(x, y)
{
	if(_font != nullptr)
	{ 
		graphic = App->fonts->Print(_string, _color, _font);
	}
	else
	{
		graphic = App->fonts->Print(_string, _color);
	}
	type = TEXT;
	color = _color;
	font = _font;
}


Text::~Text()
{

}

void Text::Draw()
{
	App->render->Blit(graphic, position.x, position.y,1,false);
}

void Text::UpdateText(char* text)
{
	if (font != nullptr)
		graphic = App->fonts->Print(text, color, font);
	else
		graphic = App->fonts->Print(text, color);
}