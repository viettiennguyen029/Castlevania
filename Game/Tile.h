#pragma once

#include "Textures.h"
#include "Game.h"

#define MAP_ID	100

class CTile
{
	int x, y;
	int left, top, right, bottom;
public:
	CTile(int _x, int _y, int _left, int _top, int _right, int _bottom);
	void Render();
};
typedef CTile* LPTILE;

