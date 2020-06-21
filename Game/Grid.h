#pragma once
#include "GameObject.h"
#include "Cell.h"

/*
Grid is a set of cells evenly and next to each other
*/
class CGrid
{
	vector<vector<LPCELL>> cells;

	int rows;			// Number of cells in a column
	int columns;		// Number of cells in a row

	int mapWidth;
	int mapHeight;

	int cellHeight;
	int cellWidth;

public:
	void Init(vector <LPGAMEOBJECT>* coObjects, int mapWidth, int mapHeight, int cellWidth, int cellHeight);
	void GetCellsContainRectangle(float const& left, float const& top,
		float const& right, float const& bottom,
		int& firstCellColumn, int& firstCellRow,
		int& lastCellColumn, int& lastCellRow);
	void Classify(LPGAMEOBJECT obj);

	// Get all the objects in cells
	void GetObjectsInRectangle(float const& left, float const& top,
		float const& right, float const& bottom,
		vector<LPGAMEOBJECT>& coObjects);
};

