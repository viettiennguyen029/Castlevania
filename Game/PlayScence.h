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
#include "Dagger.h"
#include "Black_Knight.h"
#include "Bat.h"
#include "StairTop.h"
#include "StairBottom.h"
#include "VariousStair.h"
#include "MovingPlatform.h"
#include "Boomerang.h"
#include "BreakWall.h"
#include "WallPieces.h"
#include "Board.h"
#include<vector>

#define SCREEN_WIDTH	 270
#define SCREEN_HEIGHT 270

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
	
	int mapWidth, offset_y;
	vector<LPTILE> tiledMap;
	
	vector<LPGAMEOBJECT> objects;

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