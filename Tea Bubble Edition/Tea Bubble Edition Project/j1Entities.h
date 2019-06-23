#ifndef __j1Entities__
#define __j1Entities__

#include "j1Module.h"

#include "Player.h"
#include "StaticEntity.h"
#include "p2List.h"

enum STATIC_ELEMENT_TYPE {
	CUP_DISPENSER,
	TEA_DISPENSER,
	CONDIMENT_DISPENSER,
	NONE_STATIC_ELEMENT
};

class j1Entities: public j1Module {
public:
	j1Entities();
	virtual ~j1Entities();
	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void CreatePlayer();
	StaticEntity* CreateStaticEntity(STATIC_ELEMENT_TYPE element);

	bool debug_draw = false;

private:
	Player* player = nullptr;
	p2List<StaticEntity*> static_entities;
};

#endif