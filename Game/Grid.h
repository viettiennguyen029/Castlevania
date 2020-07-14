#pragma once
#include "GameObject.h"

#define CELL_WIDTH	96
#define CELL_HEIGHT	48
class CGrid
{
	int column;
	int row;

	vector<LPGAMEOBJECT> cells[6][10];
	vector<LPGAMEOBJECT> crossObject;
	static CGrid* __instance;
public:
	// Put the object in the suitable cell
	void Classify(LPGAMEOBJECT obj); 

	// Get objects in cells[x][y] based on camera position
	void GetObjectsInGrid(vector<LPGAMEOBJECT>&objects, float left, float top, float right, float bottom);

	void Clear();

	static CGrid* GetInstance();
};

