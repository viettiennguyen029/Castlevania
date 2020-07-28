#pragma once
#include "GameObject.h"

#define CELL_WIDTH	96
#define CELL_HEIGHT	48

#define COLUMN_MAX	10
#define ROW_MAX			6
/*
	Grid is a set of cells next to each others
*/
class CGrid
{
	int column;
	int row;

	vector<LPGAMEOBJECT> cells[ROW_MAX][COLUMN_MAX];
	vector<LPGAMEOBJECT> crossObject;
	static CGrid* __instance;
public:
	// Put the object in the suitable cell
	void Classify(LPGAMEOBJECT obj); 

	void PutObjectIntoGrid(LPGAMEOBJECT obj, int row_index, int column_index);

	// Get objects in cells[x][y] based on camera position
	void GetObjectsInGrid(vector<LPGAMEOBJECT>&objects, float left, float top, float right, float bottom);

	void Clear();

	static CGrid* GetInstance();
};

