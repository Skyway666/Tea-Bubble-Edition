#pragma once
#include "Ui_element.h"

struct _TTF_Font;
class Text : public Ui_element
{
public:
	Text(int x, int y, const char* _string, SDL_Color color, _TTF_Font* font); 
	~Text();
	void Draw();
	void UpdateText(char* text);
private:

	SDL_Texture* graphic;
	SDL_Color color;
	_TTF_Font* font;
};

