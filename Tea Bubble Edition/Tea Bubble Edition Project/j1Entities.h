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
	StaticEntity* CreateStaticEntity(STATIC_ELEMENT_TYPE element, iPoint position);

	bool debug_draw = true;

private:
	Player* player = nullptr;
	p2List<StaticEntity*> static_entities;

	void ManageHover(StaticEntity* entity_check);
	bool CheckCollision(SDL_Rect rect, iPoint point) { return point.x > rect.x && point.x < rect.x + rect.w && point.y > rect.y && point.y < rect.y + rect.h;}
};

#endif