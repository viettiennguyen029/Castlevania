#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Koopas.h"
#include "Simon.h"
#include "TileMap.h"
#include "Candle.h"
#include "Whip.h"
#include "Map.h"
#include "Items.h"
#include "ItemBigHeart.h"
#include "ItemChain.h"
#include "ItemDagger.h"

using namespace std;
class CPlayScene: public CScene
{
protected: 
	CSimon* player;	// A play scene has to have player, right? 
	CWhip* whip;
	CTileMap* map;
	
	vector<LPGAMEOBJECT> objects;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

public: 
	
	CPlayScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();


	friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{	
public: 	
	void KeyState(BYTE *states);
	void OnKeyDown(int KeyCode);
	void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

