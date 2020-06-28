#include "Grid.h"
#include <unordered_set>

#define CELL_WIDTH	96
#define CELL_HEIGHT	48

void CGrid::Classify(LPGAMEOBJECT obj)
{
	float x, y;
	obj->GetPosition(x, y);

	int column = (int)(x / CELL_WIDTH);
	int row = (int)(y / CELL_HEIGHT);

	cells[row][column].push_back(obj);
}

void CGrid::GetObjects(vector<LPGAMEOBJECT>&objects, float left, float top, float right, float bottom)
{
	objects.clear();

	unordered_set<LPGAMEOBJECT> setOfObjects;

	for (int i = top; i <= bottom; i++)
	{
		for (int j = left; j <= right; j++)
		{
			for (UINT k = 0; k < cells[i][j].size(); k++)
			{
				setOfObjects.insert(cells[i][j].at(k));
			}
		}
	}

	for (auto it = setOfObjects.begin(); it != setOfObjects.end(); ++it)
		objects.push_back(*it);
}

void CGrid::Clear()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 12; j++)
		{
			cells[i][j].clear();
		}
}
