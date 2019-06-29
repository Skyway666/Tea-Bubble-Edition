#ifndef __EMPTY_CUP__
#define __EMPTY_CUP__


#include "StaticEntity.h"
#include "Player.h"
class CupDispenser :
	public StaticEntity {
public:
	CupDispenser(iPoint position);
	~CupDispenser();

	void Take(Player* player);

private:
	// Animations for static object
	Animation idle;
	//Animation taking;

	Object empty_cup;
};

#endif