#include "Grid.h"
#include <unordered_set>


void CGrid::Init(vector <LPGAMEOBJECT>* coObjects, int mapWidth, int mapHeight, int cellWidth, int cellHeight)
{
	// Get some useful information
	this->mapWidth = mapWidth;
	this->mapHeight = mapHeight;

	this->cellWidth = cellWidth;
	this->cellHeight = cellHeight;

	// Get the total of columns, rows
	this->rows = (mapHeight % cellHeight == 0) ?
		mapHeight / cellHeight :
		mapHeight / cellHeight + 1;

	this->columns = (mapWidth % cellWidth == 0) ?
		mapWidth / cellWidth :
		mapWidth / cellWidth + 1;

	// Init the cells container
	cells.resize(columns);
	for (UINT i = 0; i < cells.size(); ++i)
		cells[i].resize(rows, NULL); //Resizes the container so that it contains n elements

	// Get all the objects in the given map, in this case, it's coObjects in the Playscene

	// Put objects into suitable cell
	for (UINT i = 0; i < coObjects->size(); ++i)
		this->Classify(coObjects->at(i));
}

void CGrid::GetCellsContainRectangle(float const& left, float const& top, float const& right, float const& bottom, int& firstCellColumn, int& firstCellRow, int& lastCellColumn, int& lastCellRow)
{
	 firstCellColumn = (left < 0) ? 0 : left / cellWidth;
	firstCellRow = (top < 0) ? 0 : top / cellHeight;

	lastCellColumn = (right / cellWidth < columns) ? (right / cellWidth) : columns - 1;
	lastCellRow = (bottom / cellHeight < rows) ? (bottom / cellHeight) : rows - 1;
}

void CGrid::Classify(LPGAMEOBJECT obj)
{
	int beginCellColumn, beginCellRow, endCellColumn, endCellRow;
	float left, top, right, bottom;
	obj->GetBoundingBox(left, top, right, bottom);

	this->GetCellsContainRectangle(left, top, right, bottom,
		beginCellColumn, beginCellRow, endCellColumn, endCellRow);

	// add object to the suitable cells
	for (UINT row = beginCellRow; row <= endCellRow; row++)
		for (UINT column = beginCellColumn; column <= endCellColumn; column++)
		{
			if (cells[column][row] == NULL)
				cells[column][row] = new CCell();

			cells[column][row]->Add(obj);
		}
}

void CGrid::GetObjectsInRectangle(float const& left, float const& top, float const& right, float const& bottom, vector<LPGAMEOBJECT>& coObjects)
{
	// Find the top-left and bottom-right cell that contains the viewport's area
	int firstCellColumn, firstCellRow;		// the left-top cell that containing the rectangle's area
	int lastCellColumn, lastCellRow;		// the right-bottom cell that containing the rectangle's area
	this->GetCellsContainRectangle(left, top, right, bottom,
		firstCellColumn, firstCellRow, lastCellColumn, lastCellRow);

	// get objects to vector from cells
	unordered_set<LPGAMEOBJECT> setOfObjects;
	for (UINT row = firstCellRow; row <= lastCellRow; row++)
		for (UINT column = firstCellColumn; column <= lastCellColumn; column++)
		{
			// Empty cell
			if (cells[column][row] == NULL)
				continue;

			// Get objects containing in cell
			vector<LPGAMEOBJECT>* cellObjects = NULL;
			cells[column][row]->GetCoObjects(cellObjects);

			// Get the unique pointer to the game object
			for (UINT i = 0; i < cellObjects->size(); i++)
				setOfObjects.insert(cellObjects->at(i));
		}

	for (auto it = setOfObjects.begin(); it != setOfObjects.end(); ++it)
		coObjects.push_back(*it);
}

