#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include"Tile.h"
#include "Simon.h"
#include "Candle.h"
#include "Portal.h"
#include "Whip.h"
#include "Items.h"
#include "ItemBigHeart.h"
#include "ItemSmallHeart.h"
#include "ItemChain.h"
#include "ItemDagger.h"
#include "ItemBoomerang.h"
#include "ItemMoneyBag.h"
#include "ItemCrown.h"
#include "Dagger.h"
#include "Black_Knight.h"
#include "HunchBack.h"
#include "Bat.h"
#include "Zombie.h"
#include "StairTop.h"
#include "StairBottom.h"
#include "VariousStair.h"
#include "MovingPlatform.h"
#include "Door.h"
#include "Boomerang.h"
#include "BreakWall.h"
#include "WallPieces.h"
#include "Board.h"
#include<vector>
#include "Grid.h"

#define SCREEN_WIDTH	 270
#define SCREEN_HEIGHT 270

#define CELL_WIDTH	90
#define CELL_HEIGHT	48

#define TILE_WIDTH	32
#define TILE_HEIGHT	32

using namespace std;

class CPlayScene: public CScene
{
protected: 
	CSimon* player;	// A play scene has to have player, right? 
	CWhip* whip;
	CDagger* dagger;
	CBoomerang* boomerang;

	Board* HUD;
	
	CGrid *grid;
	int mapWidth, mapHeight;
	int offset_y; // an empty space to render HUD

	vector<LPTILE> tiledMap;	
	vector<LPGAMEOBJECT> objects;				// Base objects
	vector<LPGAMEOBJECT> updateObjects;	// The objects need be updated

	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP_INFO(string line);
	void  _ParseSection_TILE_MAP(string line);

public: 
	CPlayScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CSimon* GetPlayer() { return player; }
	CDagger* GetDagger() { return dagger; }
	CBoomerang* GetBoomerang() { return boomerang; }

	//void UseDagger();
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{	
public: 	
	void KeyState(BYTE *states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};