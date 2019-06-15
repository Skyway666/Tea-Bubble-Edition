#include "j1Module.h"
#include "Animation.h"

class SDL_Texture;

enum FADE_STATE
{
	NOTHING,
	FADING,
	UNFADING
};
class j1Transition : public j1Module
{
public:
	j1Transition();
	~j1Transition();

	//Load the assets for the transition animation
	bool Start();

	//Update animation speed
	bool Update(float dt);
	//If it is transitioning, make animation appear for the duration of the transition and turn bools to true in the middle of the animation
	bool PostUpdate();

	//Make a giant animation over all the stuff blited in the screen. The bools passed as arguments will be made true in the middle of the animation
	void Make_transition(bool* bool_1, bool* bool_2 = nullptr, bool* bool_3 = nullptr); //PROVISIONAL there should be a list and a function from this module called "AddBools_to_transition"

	bool transitioning = false;

private:

	bool* bool1 = nullptr;
	bool* bool2 = nullptr;
	bool* bool3 = nullptr;
	SDL_Texture* graphics = nullptr;
	Animation transition1;
	Animation transition2;
	FADE_STATE current_state;



};

