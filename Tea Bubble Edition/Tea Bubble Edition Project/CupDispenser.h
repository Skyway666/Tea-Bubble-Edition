#pragma once
#include "StaticEntity.h"
class CupDispenser :
	public StaticEntity {
public:
	CupDispenser(iPoint position);
	~CupDispenser();
	void Update();

	void Take(Player* player);

private:
	Animation idle;
	Animation taking;
};

