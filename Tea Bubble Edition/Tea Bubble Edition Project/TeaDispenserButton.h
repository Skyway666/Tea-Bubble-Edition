#ifndef __TEA_DISPENSER_BUTTON__
#define __TEA_DISPENSER_BUTTON__

#include "StaticEntity.h"

class TeaDispenser;

class TeaDispenserButton :
	public StaticEntity {
public:
	TeaDispenserButton(iPoint position);
	~TeaDispenserButton();

	void Take(Player* player);

	void LinkTeaDispenser(TeaDispenser* tea_dispenser) { linked_tea_dispenser = tea_dispenser; }

private:
	Animation idle;
	TeaDispenser* linked_tea_dispenser;
};

#endif