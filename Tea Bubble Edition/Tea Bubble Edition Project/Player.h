#ifndef __Player__
#define __Player__

#include "Animation.h"
#include "p2Point.h"


enum OBJECT_TYPE {
	EMPTY_CUP,
	NONE_OBJECT_TYPE
};
class Object {
public:
	OBJECT_TYPE type;
	Animation current_animation;

};




struct SDL_Texture;

class Player {
public:
	Player();
	~Player();
	void Update();
	void Draw(SDL_Texture*, float scale = 1);
	void DebugDraw();

private:

	iPoint position;
	Object object; // Functions will be called on the player to modify the contents of the oject

};

#endif