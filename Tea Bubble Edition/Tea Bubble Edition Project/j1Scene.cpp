#include "j1Scene.h"
#include "j1Fonts.h"
#include "j1Transition.h"
#include "j1Entities.h"
#include "CupDispenser.h"

#include "Brofiler/Brofiler.h"


j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& conf)
{
	LOG("Loading Scene");

	return true;
}

// Called before the first frame
bool j1Scene::Start()
{
	title = App->fonts->Load("fonts/open_sans/OpenSans-Bold.ttf", 50);
	text_body = App->fonts->Load("fonts/open_sans/OpenSans-Bold.ttf", 15);

	//kawaii_cup = App->gui->Add_window(400, 130);
	cup_dispenser = (CupDispenser*)App->entities->CreateStaticEntity(CUP_DISPENSER, iPoint(400, 130));

	return true;
}

// Used to load and unload stuff safelly
bool j1Scene::PreUpdate()
{
	
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || exit_app)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	return true;
}


void j1Scene::OnMouseEvent(UI_EVENT event, Ui_element* element)
{
	//TEST
	if(event == MOUSE_STOP_CLICK && !App->transition->transitioning)
	{ 
		
	}
	//TEST
}