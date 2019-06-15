#include "j1Scene.h"
#include "j1Gui.h"
#include "Player.h"
#include "j1Fonts.h"
#include "j1Transition.h"

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
	main_menu_background_file_name = conf.child("menu_background").attribute("file").as_string("");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{

	
	main_menu_background = App->tex->Load(main_menu_background_file_name.GetString());
	App->map->path_indicator = App->tex->Load("textures/path_indicator.png");
	title = App->fonts->Load("fonts/open_sans/OpenSans-Bold.ttf", 50);
	text_body = App->fonts->Load("fonts/open_sans/OpenSans-Bold.ttf", 15);


	//Menu setup
	Load_main_menu();



	return true;
}

// Used to load and unload stuff safelly
bool j1Scene::PreUpdate()
{
	if (App->pause && pause_menu_spawned)
	{
		Load_pause();
		pause_menu_spawned = false;
	}
	else if(!App->pause && !pause_menu_spawned)
	{
		UnLoad_pause();
		pause_menu_spawned = true;
	}

	//Execute load and unload functions safelly
	//First all unloads
		if (want_unload_main_menu)
		{
			UnLoad_main_menu();
			want_unload_main_menu = false;
		}
		if (want_unload_credits)
		{
			UnLoad_credits();
			want_unload_credits = false;
		}
		if (want_unload_options)
		{
			UnLoad_options();
			want_unload_options = false;
		}
		if (want_unload_HUD)
		{
			UnLoad_HUD();
			App->entities->ErasePlayer();
			App->audio->Play_Menu_Music();
			want_unload_HUD = false;
		}
		if (want_unload_map)
		{
			Unload_map();
			want_unload_map = false;
		}
	//Then all loads
		if (want_load_main_menu)
		{
			Load_main_menu();
			want_load_main_menu = false;
		}
		if (want_load_credits)
		{
			Load_credits();
			want_load_credits = false;
		}
		if (want_load_options)
		{
			Load_options();
			want_load_options = false;
		}
		if (want_load_HUD)
		{
			Load_HUD();
			want_load_HUD = false;
		}
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	// Lives icon management
	if (lives != nullptr)
	{
		if (App->entities->player->lives == 3)
			LivesFrame = 0;
		else if (App->entities->player->lives == 2)
			LivesFrame = 1;
		else if (App->entities->player->lives == 1)
			LivesFrame = 2;
		else if (App->entities->player->lives == 0)
			LivesFrame = 3;

		lives->animation->SetFrame(LivesFrame);
	}

	// Playtime counter
	if (hourglass != nullptr)
	{
		UpdateTime();
		hourglass_time->UpdateText(time);
		if (App->pause)
			hourglass->animation->Reset();

		if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
			ResetPlaytime();
	}

	// Update points
	if (coins != nullptr)
	{
		sprintf_s(currentcoins, 10, "x%i", App->entities->player->coins);
		current_coins->UpdateText(currentcoins);
		
		if (coin_animation)
		{
			coins->animation->Reset();	
			coin_animation = false;
		}
		if (App->pause)
			coins->animation->Reset();
	}

	if (pointsicon != nullptr)
	{
		sprintf_s(currentpoints, 10, "x%i", App->entities->player->points);
		current_points->UpdateText(currentpoints);

		if (App->entities->player->points % 10 == 0)
			pointsicon->animation->Reset();

		if (App->pause)
			pointsicon->animation->Reset();
	}

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		if (App->framerate_cap != 0)
		{
			App->framerate_cap = 0;
			App->cap = "Off";
		}
		else
		{
			App->framerate_cap = App->config_framerate_cap;
			App->cap = "On";
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && App->map->map_loaded)
	{ 
		App->LoadGame();
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN  && App->map->map_loaded)
	{
		App->have_saved_game = true;
		App->SaveGame();
	}
		


	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 50 * dt;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 50 * dt;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 50 * dt;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 50 * dt;

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT)
	{
		//tests here
	}

	// Set camera to follow the player (commented in order to debug better)
	if(App->entities->player != nullptr)
	{ 
		App->render->camera.x = -App->entities->player->position.x + 400;
		App->render->camera.y = -App->entities->player->position.y + 400;
	}

	//Camera limit (don't let player see ugly stuff)
	if (App->render->camera.y < App->map->data.camera_y_limit)
		App->render->camera.y = App->map->data.camera_y_limit;



	App->map->Draw();
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

	App->tex->UnLoad(App->map->bone_graphics);
	return true;
}

void j1Scene::Change_to_map(int _map)
{
	if(App->map->map_loaded)
	{ 
		App->entities->EraseEntities();
		App->collision->Erase_Non_Player_Colliders();
		App->entities->Clear_waiting_list();
		App->map->CleanUp();
	}
	if (_map == 0)
	{
		App->map->Load("Map 1.tmx");
		App->map->map = 0;
	}
	else if (_map == 1)
	{
		App->map->Load("Map 2.tmx");
		App->map->map = 1;
	}
	App->pathfinding->SetMap();
	App->entities->Spawn_waiting_entities();
	App->loading_frame = true;
}

void j1Scene::Unload_map()
{
	BROFILER_CATEGORY("Unload Map", 0xFF6495ED);
	App->entities->EraseEntities();
	App->collision->Erase_Non_Player_Colliders();
	App->entities->Clear_waiting_list();
	App->map->CleanUp();
	App->map->map = -1;
	App->pause = false;
}
void j1Scene::Load_main_menu()
{
	//Start button (All of it could be the child of start to simplify code)
	start = App->gui->Add_button(370, 250, (j1Module*)this);
	Text* text_to_link = App->gui->Add_text(0, 0, "START");
	start->Link_ui_element(text_to_link, 100, 30);
	//Continue button
	continuee = App->gui->Add_button(370, 350, (j1Module*)this);
	text_to_link = App->gui->Add_text(0, 0, "CONTINUE");
	continuee->Link_ui_element(text_to_link, 75, 30);	
	//Options button
	options = App->gui->Add_button(370, 450, (j1Module*)this);
	text_to_link = App->gui->Add_text(0, 0, "OPTIONS");
	options->Link_ui_element(text_to_link, 85, 30);	
	//Credits button
	credits = App->gui->Add_button(370, 550, (j1Module*)this);
	text_to_link = App->gui->Add_text(0, 0, "CREDITS");
	credits->Link_ui_element(text_to_link, 85, 30);
	//Exit button
	exit = App->gui->Add_button(370, 650, (j1Module*)this);
	text_to_link = App->gui->Add_text(0, 0, "EXIT");
	exit->Link_ui_element(text_to_link, 105, 30);

	if (!App->have_saved_game)
	{
		continuee->SetActive(false);
	}

	

	App->gui->Set_backgrond(main_menu_background);
}

void j1Scene::UnLoad_main_menu()
{
	App->gui->Erase_Ui_element(start);
	start = nullptr;
	App->gui->Erase_Ui_element(continuee);
	continuee = nullptr;
	App->gui->Erase_Ui_element(exit);
	exit = nullptr;
	App->gui->Erase_Ui_element(credits);
	credits = nullptr;
	App->gui->Erase_Ui_element(options);
	options = nullptr;

	App->gui->Set_backgrond(nullptr);
}

void j1Scene::Load_credits()
{
	//Text in credits 
	main_title = App->gui->Add_text(10, 40, "Credits", { 255,255,255,255 }, title); //Main text

	sprintf_s(max_points_text, 10, "%i", App->max_punctuation);

	Text* credits_1 = App->gui->Add_text(0, 0, "Thanks for playing 'Who let the dog Jump?', your maximum score was:", { 255,255,255,255 }, text_body);
	Text* max_points = App->gui->Add_text(0, 0, max_points_text, { 255,255,255,255 }, text_body);
	Text* credits_2 = App->gui->Add_text(0, 0, "Game by Lucas García Mateu and Genís Bayó Salgado", { 255,255,255,255 }, text_body); 

	Text* licence   = App->gui->Add_text(0, 0, "Licence", { 255,255,255,255 }, title);

	Text* licence_1 = App->gui->Add_text(0, 0, "Copyright (c) 2017 Lucas García Mateu, Genís Bayó Salgado", { 255,255,255,255 }, text_body);
	Text* licence_2 = App->gui->Add_text(0, 0, "Permission is hereby granted, free of charge, to any person obtaining a copy", { 255,255,255,255 }, text_body);
	Text* licence_3 = App->gui->Add_text(0, 0, "of this software and associated documentation files (the 'Software'), to deal", { 255,255,255,255 }, text_body);
	Text* licence_4 = App->gui->Add_text(0, 0, "in the Software without restriction, including without limitation the rights", { 255,255,255,255 }, text_body);
	Text* licence_5 = App->gui->Add_text(0, 0, "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell", { 255,255,255,255 }, text_body);
	Text* licence_6 = App->gui->Add_text(0, 0, "copies of the Software, and to permit persons to whom the Software is", { 255,255,255,255 }, text_body);
	Text* licence_7 = App->gui->Add_text(0, 0, "furnished to do so, subject to the following conditions:", { 255,255,255,255 }, text_body);
	Text* licence_8 = App->gui->Add_text(0, 0, "Copyright (c) 2017 Lucas García Mateu, Genís Bayó Salgado", { 255,255,255,255 }, text_body);

	Text* licence_9 = App->gui->Add_text(0, 0, "The above copyright notice and this permission notice shall be included in all", { 255,255,255,255 }, text_body);
	Text* licence_10 = App->gui->Add_text(0, 0, "copies or substantial portions of the Software.", { 255,255,255,255 }, text_body);

	Text* licence_11 = App->gui->Add_text(0, 0, "THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR", { 255,255,255,255 }, text_body);
	Text* licence_12 = App->gui->Add_text(0, 0, "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,", { 255,255,255,255 }, text_body);
	Text* licence_13 = App->gui->Add_text(0, 0, "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE", { 255,255,255,255 }, text_body);
	Text* licence_14 = App->gui->Add_text(0, 0, "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER", { 255,255,255,255 }, text_body);
	Text* licence_15 = App->gui->Add_text(0, 0, "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,", { 255,255,255,255 }, text_body);
	Text* licence_16 = App->gui->Add_text(0, 0, "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE", { 255,255,255,255 }, text_body);
	Text* licence_17 = App->gui->Add_text(0, 0, "SOFTWARE.", { 255,255,255,255 }, text_body);

	//Link all texts to title
		//Credits and score
		main_title->Link_ui_element(credits_1, 20, 80);
		main_title->Link_ui_element(max_points, 560, 80);
		main_title->Link_ui_element(credits_2, 20, 120);
		
		//Licence
		main_title->Link_ui_element(licence, 0, 170);
		
		main_title->Link_ui_element(licence_1, 20, 250);
		main_title->Link_ui_element(licence_2, 20, 290);
		main_title->Link_ui_element(licence_3, 20, 310);
		main_title->Link_ui_element(licence_4, 20, 330);
		main_title->Link_ui_element(licence_5, 20, 350);
		main_title->Link_ui_element(licence_6, 20, 370);
		main_title->Link_ui_element(licence_7, 20, 390);
		main_title->Link_ui_element(licence_8, 20, 410);

		main_title->Link_ui_element(licence_9, 20, 450);
		main_title->Link_ui_element(licence_10, 20, 470);

		main_title->Link_ui_element(licence_11, 20, 510);
		main_title->Link_ui_element(licence_12, 20, 530);
		main_title->Link_ui_element(licence_13, 20, 550);
		main_title->Link_ui_element(licence_14, 20, 570);
		main_title->Link_ui_element(licence_15, 20, 590);
		main_title->Link_ui_element(licence_16, 20, 610);
		main_title->Link_ui_element(licence_17, 20, 630);


	//Button to exit to main menu from credits
	Text* text_to_link = App->gui->Add_text(0, 0, "MAIN MENU");
	exit_main_menu_fc = App->gui->Add_button(750, 650, (j1Module*)this);
	exit_main_menu_fc->Link_ui_element(text_to_link, 70, 30);

	App->gui->Set_backgrond(nullptr);
}
void j1Scene::UnLoad_credits()
{
	App->gui->Erase_Ui_element(main_title);
	main_title = nullptr;
	App->gui->Erase_Ui_element(exit_main_menu_fc);
	exit_main_menu_fc = nullptr;

	App->gui->Set_backgrond(nullptr);
}

void j1Scene::Load_pause()
{
	//Text* titola = App->gui->Add_text(0, 0, "PAUSE MENU", title);
	Pause_Window = App->gui->Add_window(300, 100);

	resume = App->gui->Add_button(0, 0, (j1Module*)this);
	Text* text_to_link = App->gui->Add_text(0, 0, "RESUME");
	resume->Link_ui_element(text_to_link, 90, 30);

	exit_main_menu_fg = App->gui->Add_button(0, 0, (j1Module*)this);
	text_to_link = App->gui->Add_text(0, 0, "MAIN MENU");
	exit_main_menu_fg->Link_ui_element(text_to_link, 70, 30);

	Pause_Window->Link_ui_element(resume, 90, 100);
	Pause_Window->Link_ui_element(exit_main_menu_fg, 90, 300);
	//Pause_Window->Link_ui_element(titola, 120, 30);
}

void j1Scene::UnLoad_pause()
{
	App->gui->Erase_Ui_element(Pause_Window);
	Pause_Window = nullptr;
	resume = nullptr;
	exit_main_menu_fg = nullptr;
}

void j1Scene::Load_options()
{
	//Create window elements
	upper_music_volume = App->gui->Add_button(900, 400, (j1Module*)this, UPPER);
	lower_music_volume = App->gui->Add_button(400, 400, (j1Module*)this, LOWER);

	upper_fx_volume = App->gui->Add_button(900, 600, (j1Module*)this, UPPER);
	lower_fx_volume = App->gui->Add_button(400, 600, (j1Module*)this, LOWER);

	exit_main_menu_fo = App->gui->Add_button(0, 600, (j1Module*)this);
	Text* text_to_link = App->gui->Add_text(0, 0, "MAIN MENU");
	exit_main_menu_fo->Link_ui_element(text_to_link, 70, 30);

	music_volume = App->gui->Add_StatBar(565, 410, 300, 50, &App->audio->music_volume, MIX_MAX_VOLUME);
	text_to_link = App->gui->Add_text(0, 0, "MUSIC VOLUME");
	//Create label for text
	Icon* pretty_label = App->gui->Add_icon(0, 0);
	pretty_label->anim.PushBack({ 939, 1139, 362, 141 });
	pretty_label->anim.loop = false;
	pretty_label->animation = &pretty_label->anim;
	pretty_label->scale = 0.7;
	//Link label to text
	text_to_link->Link_ui_element(pretty_label, -35, -30);

	music_volume->Link_ui_element(text_to_link, 0, -80);

	fx_volume = App->gui->Add_StatBar(565, 610, 300, 50, &App->audio->fx_volume, MIX_MAX_VOLUME);
	text_to_link = App->gui->Add_text(0, 0, "FX VOLUME");
	//Create label for text
	pretty_label = App->gui->Add_icon(0, 0);
	pretty_label->anim.PushBack({ 939, 1139, 362, 141 });
	pretty_label->anim.loop = false;
	pretty_label->animation = &pretty_label->anim;
	pretty_label->scale = 0.7;
	//Link label to text
	text_to_link->Link_ui_element(pretty_label, -60, -30);

	fx_volume->Link_ui_element(text_to_link, 20, -80);

	App->gui->Set_backgrond(main_menu_background);

}

void j1Scene::UnLoad_options()
{
	App->gui->Erase_Ui_element(upper_music_volume);
	upper_music_volume = nullptr;
	App->gui->Erase_Ui_element(lower_music_volume);
	lower_music_volume = nullptr;
	App->gui->Erase_Ui_element(upper_fx_volume);
	upper_fx_volume = nullptr;
	App->gui->Erase_Ui_element(lower_fx_volume);
	lower_fx_volume = nullptr;
	App->gui->Erase_Ui_element(exit_main_menu_fo);
	exit_main_menu_fo = nullptr;
	App->gui->Erase_Ui_element(music_volume);
	music_volume = nullptr;
	App->gui->Erase_Ui_element(fx_volume);
	fx_volume = nullptr;
}
void j1Scene::OnMouseEvent(UI_EVENT event, Ui_element* element)
{
	//TEST
	if(event == MOUSE_STOP_CLICK && !App->transition->transitioning)
	{ 
		//Main menu
			if (element == start)
			{
				//Play button sound effect
				App->audio->PlayFx(4);
				//Game loading
				Change_to_map(0);
				App->entities->AddEntity(ENTITY_TYPES::PLAYER, App->map->data.player_starting_value.x, App->map->data.player_starting_value.y); 
				//Unload main menu adn load hud
				App->transition->Make_transition(&want_unload_main_menu, &want_load_HUD);
				//Play game music
				App->audio->Play_Game_Music();
			}
			if (element == continuee)
			{	
				continuing = true;
				//Play button sound effect
				App->audio->PlayFx(4);
				//Load saved game
				App->LoadGame();
				//Create new player when continuing from menu
				App->entities->AddEntity(ENTITY_TYPES::PLAYER, App->map->data.player_starting_value.x, App->map->data.player_starting_value.y);
				//Unload main menu adn load hud
				App->transition->Make_transition(&want_unload_main_menu, &want_load_HUD);
				//Play game music
				App->audio->Play_Game_Music();
			}
			if (element == exit)
			{
				//Play button sound effect
				App->audio->PlayFx(4);
				//Exit app
				App->transition->Make_transition(&exit_app);
			}
			if (element == credits)
			{
				//Play button sound effect
				App->audio->PlayFx(4);
				//Unload main menu and Load credits
				App->transition->Make_transition(&want_unload_main_menu, &want_load_credits);
			}
			if (element == options)
			{
				//Play button sound effect
				App->audio->PlayFx(4);
				//Unload main menu and load options
				App->transition->Make_transition(&want_unload_main_menu, &want_load_options);
			}
		//Pause menu
			if (element == resume)
			{
				App->pause = false;
			}
			if (element == exit_main_menu_fg)
			{	
				//Load main menu and Unload HUD and Game Unloading
				App->transition->Make_transition(&want_load_main_menu, &want_unload_HUD, &want_unload_map);
			}

		//Credits menu
			if (element == exit_main_menu_fc)
			{
				//Play button sound effect
				App->audio->PlayFx(4);
				//Unload credits and Load main menu
				App->transition->Make_transition(&want_unload_credits, &want_load_main_menu);
			}
		//Options menu
			if (element == upper_music_volume)
			{
				App->audio->Modify_music_volume(10); // "10" could be a value loaded from xml
			}
			if (element == lower_music_volume)
			{
				App->audio->Modify_music_volume(-10);
			}
			if (element == upper_fx_volume)
			{
				App->audio->Modify_fx_volume(10);
				App->audio->PlayFx(1);
			}
			if (element == lower_fx_volume)
			{
				App->audio->Modify_fx_volume(-10);
				App->audio->PlayFx(1);
			}
			if (element == exit_main_menu_fo) 
			{
				//Play button sound effect
				App->audio->PlayFx(4);
				//Unload main menu and load options
				App->transition->Make_transition(&want_load_main_menu, &want_unload_options);
			}


	}
	//TEST
}

void j1Scene::Load_HUD()
{
	BROFILER_CATEGORY("Load HUD", 0xFF008B8B);

	hourglass = App->gui->Add_icon(920, 580);
	coins = App->gui->Add_icon(-20, -20);
	lives = App->gui->Add_icon(0, 620);
	pointsicon = App->gui->Add_icon(850, -20);

	int row = 0;
	for (int i = 0; i < 16; i++)
		hourglass->anim.PushBack({ 229 * i, 345 * row, 229, 345 });
	row++;
	for (int i = 0; i < 16; i++)
		hourglass->anim.PushBack({ 229 * i, 345 * row, 229, 345 });
	row++;
	for (int i = 0; i < 14; i++)
		hourglass->anim.PushBack({ 229 * i, 345 * row, 229, 345 });

	hourglass->animation_speed = 0.2;
	hourglass->anim.speed = 0.2;
	hourglass->anim.loop = true;
	hourglass->animation = &hourglass->anim;
	hourglass->scale = 0.5;

	row++;
	for (int i = 0; i < 4; i++)
		coins->anim.PushBack({ 229 * i, 345 * row, 229, 345 });
	for (int i = 3; i >= 0; i--)
		coins->anim.PushBack({ 229 * i, 345 * row, 229, 345 });

	coins->animation_speed = 0.3;
	coins->anim.speed = 0.3;
	coins->anim.loop = false;
	coins->animation = &coins->anim;
	coins->scale = 0.5;

	for (int i = 3; i < 7; i++)
		lives->anim.PushBack({ 229 * 2 * i, 345 * row, 229 * 2, 345 });
	
	lives->anim.loop = false;
	lives->anim.manual = true;
	lives->animation = &lives->anim;
	lives->scale = 0.5;


	pointsicon->anim.PushBack({ 229 * 14, 345 * row, 229, 345 });

	pointsicon->anim.loop = false;
	pointsicon->animation = &pointsicon->anim;
	pointsicon->scale = 0.5;

	hourglass_time = App->gui->Add_text(0, 0, "00:00:00");
	hourglass->Link_ui_element(hourglass_time, -80, 120);
	playtime.Start();

	current_coins = App->gui->Add_text(0, 0, "x0");
	coins->Link_ui_element(current_coins, 65, 85);

	current_points = App->gui->Add_text(0, 0, "x0");
	pointsicon->Link_ui_element(current_points, 10, 135);

	if (!continuing)
		ResetPlaytime();

	playtime.ResumeFromSec(s);

	continuing = false;
}

void j1Scene::UnLoad_HUD()
{
	App->gui->Erase_Ui_element(hourglass);
	hourglass = nullptr;
	App->gui->Erase_Ui_element(coins);
	coins = nullptr;
	App->gui->Erase_Ui_element(lives);
	lives = nullptr;
	App->gui->Erase_Ui_element(pointsicon);
	pointsicon = nullptr;
}

void j1Scene::UpdateTime()
{
	if (!App->pause)
	{
		if (s >= 10)
		{
			s = 0;
			s2++;
			playtime.Start();
		}
		if (s2 >= 6)
		{
			s2 = 0;
			m++;
		}
		if (m >= 10)
		{
			m = 0;
			m2++;
		}
		if (m2 >= 6)
		{
			m2 = 0;
			h++;
		}
		if (h >= 10)
		{
			h = 0;
			h2++;
		}
		if (h2 >= 10)
		{
			s = 0;
			s2 = 0;
			m = 0;
			m2 = 0;
			h = 0;
			h2 = 0;
		}

		int i = 0;
		time[i] = h2 + 48;
		i++;
		time[i] = h + 48;
		i++;
		time[i] = 58;
		i++;
		time[i] = m2 + 48;
		i++;
		time[i] = m + 48;
		i++;
		time[i] = 58;
		i++;
		time[i] = s2 + 48;
		i++;
		time[i] = s + 48;
		i++;
		time[i] = '\0';

		s = playtime.ReadSec();
	}
	else
	{
		playtime.ResumeFromSec(s);
	}
}

void j1Scene::ResetPlaytime()
{
	int i = 0;
	time[i] = 48;
	i++;
	time[i] = 48;
	i++;
	time[i] = 58;
	i++;
	time[i] = 48;
	i++;
	time[i] = 48;
	i++;
	time[i] = 58;
	i++;
	time[i] = 48;
	i++;
	time[i] = 48;
	i++;
	time[i] = '\0';
	s = 0;
	s2 = 0;
	m = 0;
	m2 = 0;
	h = 0;
	h2 = 0;
	playtime.Start();
}

bool j1Scene::Save(pugi::xml_node& data) const
{
	data.append_attribute("s") = s;
	data.append_attribute("s2") = s2;
	data.append_attribute("m") = m;
	data.append_attribute("m2") = m2;
	data.append_attribute("h") = h;
	data.append_attribute("h2") = h2;

	return true;
}

bool j1Scene::Load(pugi::xml_node& data)
{
	s = data.attribute("s").as_int();
	s2 = data.attribute("s2").as_int();
	m = data.attribute("m").as_int();
	m2 = data.attribute("m2").as_int();
	h = data.attribute("h").as_int();
	h2 = data.attribute("h2").as_int();

	playtime.ResumeFromSec(s);

	return true;
}