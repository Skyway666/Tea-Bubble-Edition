
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Transition.h"

j1Transition::j1Transition()
{
	name.create("transition");
}


j1Transition::~j1Transition()
{

}

bool j1Transition::Start()
{
	graphics = App->tex->Load("textures/Sandstorm.png");

	transition1.PushBack({ 0,0,1024,768 });
	transition1.PushBack({ 1024,0,1024,768 });
	transition1.PushBack({ 1024*2,0,1024,768 });
	transition1.PushBack({ 1024*3,0,1024,768 });
	transition1.PushBack({ 0,768,1024,768 });
	transition1.PushBack({ 1024,768,1024,768 });
	transition1.PushBack({ 1024 * 2,768,1024,768 });
	transition1.PushBack({ 1024 * 3,768,1024,768 });
	transition1.PushBack({ 0,768*2,1024,768 });
	transition1.PushBack({ 1024,768*2,1024,768 });

	transition1.loop = false;
	transition1.speed = 0.5;

	transition2.PushBack({ 1024,768 * 2,1024,768 });
	transition2.PushBack({ 0,768 * 2,1024,768 });
	transition2.PushBack({ 1024 * 3,768,1024,768 });
	transition2.PushBack({ 1024 * 2,768,1024,768 });
	transition2.PushBack({ 1024,768,1024,768 });
	transition2.PushBack({ 0,768,1024,768 });
	transition2.PushBack({ 1024 * 3,0,1024,768 });
	transition2.PushBack({ 1024 * 2,0,1024,768 });
	transition2.PushBack({ 1024,0,1024,768 });
	transition2.PushBack({ 0,0,1024,768 });

	transition2.loop = false;
	transition2.speed = 0.25;

	current_state = NOTHING;
	return true;
}

bool j1Transition::Update(float dt)
{
	transition1.speed = 0.25*dt;
	transition2.speed = 0.25*dt;

	return true;
}
bool j1Transition::PostUpdate()
{
	switch (current_state) {

		case NOTHING:
		{
			transition1.Reset();
			break;
		}

		case FADING:
		{
			App->render->Blit(graphics, 0, 0, 1, false, &transition1.GetCurrentFrame());

			if (transition1.Finished())
			{
				current_state = UNFADING;
				if(bool1 != nullptr)
				*bool1= true;
				if (bool2 != nullptr)
				*bool2 = true;
				if (bool3 != nullptr)
				*bool3 = true;

				transition2.Reset();
			}
			break;
		}
		case UNFADING:
		{
			App->render->Blit(graphics, 0, 0, 1, false, &transition2.GetCurrentFrame());
			if (transition2.Finished())
			{
				current_state = NOTHING;
				transitioning = false;
			}
			break;
		}
	}


	return true;
}

void j1Transition::Make_transition(bool* bool_1, bool* bool_2, bool* bool_3)
{
	bool1 = bool_1;
	bool2= bool_2;
	bool3 = bool_3;
	current_state = FADING;
	transitioning = true;
}