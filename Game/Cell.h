#pragma once
#include "GameObject.h"

class CCell
{
	vector<LPGAMEOBJECT> coObjects; // collidable objects in a cell

public:
	void Add(LPGAMEOBJECT object);

	// Return the given vector all the objects of the cell
	void GetCoObjects(vector<LPGAMEOBJECT>*& coObjects) { coObjects = &(this->coObjects); }
};
typedef CCell* LPCELL;