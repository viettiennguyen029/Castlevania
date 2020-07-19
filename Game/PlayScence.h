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
#include "ItemHolyWater.h"
#include "ItemInvisibility.h"
#include "ItemAxe.h"
#include  "ItemMeat.h"
#include "ItemCrown.h"
#include "Dagger.h"
#include "Black_Knight.h"
#include "HunchBack.h"
#include"Skeleton.h"
#include "Raven.h"
#include "Bones.h"
#include"Ghost.h"
#include "Bat.h"
#include "PhantomBat.h"
#include "Zombie.h"
#include "StairTop.h"
#include "StairBottom.h"
#include "VariousStair.h"
#include "MovingPlatform.h"
#include "Door.h"
#include "Boomerang.h"
#include "HolyWater.h"
#include "Axe.h"
#include "BreakWall.h"
#include "WallPieces.h"
#include "Board.h"
#include<vector>
#include "Grid.h"

#define SCREEN_WIDTH	 270
#define SCREEN_HEIGHT 270

#define TILE_WIDTH	32
#define TILE_HEIGHT	32

using namespace std;

class CPlayScene: public CScene
{
protected: 
	CSimon* player;	// A play scene has to have player, right? 
	
	Board* HUD;
	
	CGrid* grid = CGrid::GetInstance();
	int mapWidth, mapHeight;
	int offset_y; // an empty space to render HUD
	
	vector<LPTILE> tiledMap;	
	vector<LPGAMEOBJECT> objects;				// Base objects
	vector<LPGAMEOBJECT> invisibleObjects;  // Objects that trigger a condition to turn visible on
	vector<LPGAMEOBJECT> updateObject;  // The objects need to be updated

	void _ParseSection_SCENE_TEXTURES(string line);
	void _ParseSection_SCENE_SPRITES(string line);
	void _ParseSection_SCENE_ANIMATIONS(string line);
	void _ParseSection_SCENE_ANIMATION_SETS(string line);
	void _ParseSection_SCENE_PLAYER(string line);
	void _ParseSection_SCENE_OBJECTS(string line);
	void _ParseSection_SCENE_MAP_INFO(string line);
	void _ParseSection_SCENE_TILE_MAP(string line);

	void _ReleaseSection_SCENE_TEXTURES(string line);
	void _ReleaseSection_SCENE_SPRITES(string line);
	void _ReleaseSection_SCENE_ANIMATIONS(string line);
	void _ReleaseSection_SCENE_ANIMATION_SETS(string line);

public: 
	CPlayScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CSimon* GetPlayer() { return player; }
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{	
public: 	
	void KeyState(BYTE *states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};