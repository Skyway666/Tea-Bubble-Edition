#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "Chronometer.h"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Map;
class j1Gui;
class j1Fonts;
class j1Transition;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	// Modules
	j1Window*			win = nullptr;
	j1Input*			input = nullptr;
	j1Render*			render = nullptr;
	j1Textures*			tex = nullptr;
	j1Audio*			audio = nullptr;
	j1Scene*			scene = nullptr;
	j1Map*				map = nullptr;
	j1Gui*				gui = nullptr;
	j1Fonts*			fonts = nullptr;
	j1Transition*       transition = nullptr;


private:

	p2List<j1Module*>	modules;
	uint				frames;
	float				dt;
	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;

	mutable bool		want_to_save;
	bool				want_to_load;
	p2SString			load_game;
	mutable p2SString	save_game;

	// Framerate management

	float				frame_ms;
	double				expected_delay;
	double				real_delay;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	uint32				frame_count;
	j1PerfTimer			timer;
	j1PerfTimer			ptimer;
	j1PerfTimer			frame_time;
	j1Timer				startup_time;
	j1Timer				last_sec_frame_time;

public:

	uint	framerate_cap = 30;
	char*	Vsync = "On";
	char*	cap = "On";
	bool	pause = false;
	bool	started = false;
	bool    loading_frame; //If it is a loading frame, tell the player
	int		security_frames = 0; //Let 3 frames pass after a loading frame
	bool	have_saved_game = false; //Game has been already saved or not
	int max_punctuation = 0;
};

extern j1App* App;

#endif