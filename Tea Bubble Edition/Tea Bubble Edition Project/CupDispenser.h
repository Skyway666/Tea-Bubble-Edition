#pragma once
#include "StaticEntity.h"
class CupDispenser :
	public StaticEntity {
public:
	CupDispenser();
	~CupDispenser();
	void Update();

private:
	Animation idle;
	Animation taking;
};

