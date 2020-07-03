#include "Grid.h"
#include <unordered_set>
#include "Brick.h"

#define CELL_WIDTH	96
#define CELL_HEIGHT	48

void CGrid::Classify(LPGAMEOBJECT obj)
{
	int beginCellColumn, beginCellRow, endCellColumn, endCellRow;
	float l, t, r, b;
	obj->GetBoundingBox(l, t, r, b);

	//this->GetCellsContainRectangle(left, top, right, bottom, beginCellColumn, beginCellRow, endCellColumn, endCellRow);

	beginCellColumn = (int)(l / CELL_WIDTH);
	beginCellRow = (int)(t / CELL_HEIGHT);

	endCellColumn = (int)(r / CELL_WIDTH);
	endCellRow = (int)(b / CELL_HEIGHT);

	for (int i = beginCellRow; i <= endCellRow; i++)
	{
		for (int j = beginCellColumn; j <= endCellColumn; j++)
		{
			cells[i][j].push_back(obj);
		}
	}

	//cells[row][column].push_back(obj);

	// add object to the suitable cells
	
}

void CGrid::GetObjects(vector<LPGAMEOBJECT>&updateobjects, float left, float top, float right, float bottom)
{
	//updateobjects.clear();
	//objects.clear();
	//crossObject.clear();
	
	//unordered_set<LPGAMEOBJECT> setOfObjects;

	int firstCellColumn, firstCellRow;		// the left-top cell that containing the rectangle's area
	int lastCellColumn, lastCellRow;		// the right-bottom cell that containing the rectangle's area

	firstCellColumn = (int) left / CELL_WIDTH;
	firstCellRow = (int) top / CELL_HEIGHT;

	lastCellColumn = (int)(right) / CELL_WIDTH;
	lastCellRow = (int)(bottom) / CELL_HEIGHT;

	for (int row = firstCellRow; row <= lastCellRow + 1; row++)
	{
		for (int column = firstCellColumn; column <= lastCellColumn + 1; column++)
		{
			for (UINT k = 0; k < cells[row][column].size(); k++)
			{
				// Check the object if is in the vector or not !
				if ((find(updateobjects.begin(), updateobjects.end(), cells[row][column].at(k)) != updateobjects.end()== false))
				{
					if (cells[row][column].at(k)->isVisible())
					{
						updateobjects.push_back(cells[row][column].at(k));
					}
				}
			}
		}
	}
}

void CGrid::Clear()
{
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 10; j++)
		{
			cells[i][j].clear();
		}
}

CGrid* CGrid::__instance = NULL;

CGrid* CGrid::GetInstance()
{
	if (__instance == NULL) __instance = new CGrid();
	return __instance;
}
