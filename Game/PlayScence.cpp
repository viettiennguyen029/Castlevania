#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);	
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_OBJECTS					6
#define SCENE_SECTION_MAP_INFO				7
#define SCENE_SECTION_TILE_MAP				8

#define OBJECT_TYPE_SIMON						0
#define OBJECT_TYPE_BRICK						1
#define OBJECT_TYPE_CANDLE					2

#define OBJECT_TYPE_ITEM_BIG_HEART			4
#define OBJECT_TYPE_ITEM_SMALL_HEART	44
#define OBJECT_TYPE_ITEM_CHAIN				5
#define OBJECT_TYPE_ITEM_MONEY_BAG		10

#define OBJECT_TYPE_ITEM_DAGGER			6
#define OBJECT_TYPE_ITEM_BOOMERANG 61

#define OBJECT_TYPE_DAGGER					7
#define OBJECT_TYPE_BOOMERANG			71

#define OBJECT_TYPE_ZOMBIE					63
#define OBJECT_TYPE_BLACK_KNIGHT		8
#define OBJECT_TYPE_BAT							9

#define OBJECT_TYPE_VARIOUS_STAIR	-1
#define OBJECT_TYPE_STAIR_BOTTOM	-2
#define OBJECT_TYPE_STAIR_TOP			-3

#define OBJECT_TYPE_MOVING_PLATFORM	30
#define OBJECT_TYPE_CROWN_ITEM				40
#define OBJECT_TYPE_PORTAL						50
#define OBJECT_TYPE_BREAK_WALL				90
#define OBJECT_TYPE_WALL_PIECES				91

#define MAX_SCENE_LINE 1024

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;
	CItems* items = CItems::GetInstance();

	switch (object_type)
	{	
	case OBJECT_TYPE_SIMON:
	{
		if (player != NULL)
		{
			DebugOut(L"[ERROR] SIMON object was created before! ");
			return;
		}
		
		//obj = new CSimon(x,y);
		obj = CSimon::GetInstance();
		player = (CSimon*)obj;
		
		DebugOut(L"[INFO] Player object created!\n");
		break;
	}

	case OBJECT_TYPE_BRICK:
	{
		int width = atoi(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());
		obj = new CBrick();
		obj->SetWidth(width);
		obj->SetHeight(height);
		break;
	}
	
	case OBJECT_TYPE_DAGGER:
	{
		obj = new CDagger();
		dagger = (CDagger*)obj;
		obj->SetVisible(false);
		break;
	}

	case OBJECT_TYPE_BOOMERANG:
	{
		obj = new CBoomerang();
		boomerang = (CBoomerang*)obj;
		obj->SetVisible(false);
		break;
	}

	case OBJECT_TYPE_MOVING_PLATFORM: obj = new CMovingPlatform(); break;

	case OBJECT_TYPE_CROWN_ITEM: obj = new ItemCrown(); break;

	case OBJECT_TYPE_BREAK_WALL: obj = new CBreakWall(x,y); break;

	case OBJECT_TYPE_WALL_PIECES: 
	{
		obj = new CWallPiece();		
		CWallPieces::GetInstance()->AddPiece((CWallPiece*)obj);
		break;
	}
	case OBJECT_TYPE_BAT:
	{
		obj = new CBat(x,y);
		break;
	}	
	case OBJECT_TYPE_BLACK_KNIGHT: 
	{
		obj = new CBlack_Knight(x,y); 
		break;
	}

	case OBJECT_TYPE_ZOMBIE:
	{
		obj = new CZombie();
		break;
	}

	case OBJECT_TYPE_CANDLE: 
	{
		 int it = atoi(tokens[4].c_str());
		 int state = atoi(tokens[5].c_str());
		obj = new CCandle();	
		obj->SetState(state);
		obj->SetItemId(it);		
		break;
	}

	case OBJECT_TYPE_ITEM_BIG_HEART: 
	{
		obj = new ItemBigHeart();
		//CItems::GetInstance()->AddItem((int)CGameObject::ItemType::BIG_HEART, obj);
		//CItems::GetInstance()->AddItem((int)ItemType::BIG_HEART, obj);
		items->AddItem((int)ItemType::BIG_HEART, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_SMALL_HEART:
	{
		obj = new ItemSmallHeart ();
		items->AddItem((int)ItemType::SMALL_HEART, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_CHAIN:
	{
		 obj = new ItemChain();
		 items->AddItem((int)ItemType::CHAIN, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_DAGGER:
	{
		obj = new ItemDagger();
		//CItems::GetInstance()->AddItem((int)CGameObject::ItemType::DAGGER, obj);
		//CItems::GetInstance()->AddItem((int)ItemType::DAGGER, obj);
		items->AddItem((int)ItemType::DAGGER, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_MONEY_BAG:
	{
		obj = new ItemMoneyBag();
		items->AddItem((int)ItemType::MONEY_BAG, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_BOOMERANG:
	{
		obj = new ItemBoomerang();
		items->AddItem((int)ItemType::BOOMERANG, obj);
		break;
	}

	case OBJECT_TYPE_VARIOUS_STAIR:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int nx = atoi(tokens[6].c_str());
		obj = new CVariousStair(x, y, r, b);		
		obj->SetOrientation(nx);
		break;
	}

	case OBJECT_TYPE_STAIR_BOTTOM:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int nx = atoi(tokens[6].c_str());		
		obj = new CStairBottom(x,y,r,b);
		obj->SetOrientation(nx);
		break;
	}

	case OBJECT_TYPE_STAIR_TOP:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int nx = atoi(tokens[6].c_str());
		obj = new CStairTop(x, y, r, b);	
		obj->SetOrientation(nx);
		break;
	}


	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;

	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CPlayScene::_ParseSection_MAP_INFO(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 3) return; // skip invalid lines
	this->mapWidth = atoi(tokens[0].c_str());
	this->mapHeight = atoi(tokens[1].c_str());
	this->offset_y= atoi(tokens[2].c_str());
}

void CPlayScene::_ParseSection_TILE_MAP(string line)
{
	int cureneMapId = (CGame::GetInstance()->GetSceneId());
	LPDIRECT3DTEXTURE9 tilesheet = CTextures::GetInstance()->Get(cureneMapId);
	D3DSURFACE_DESC surfaceDesc;
	int level = 0;
	tilesheet->GetLevelDesc(level, &surfaceDesc);

	//int nums_rowToRead = surfaceDesc.Height / TILE_HEIGHT;
	int nums_colToRead = surfaceDesc.Width / TILE_WIDTH;
	
	vector<string> map_tokens = split(line);
	
	for (int i = 0; i < map_tokens.size(); i++)
	{
		RECT rectTile;
		int index = atoi(map_tokens[i].c_str());
		rectTile.left = (index % nums_colToRead) * TILE_WIDTH;
		rectTile.top = (index / nums_colToRead) * TILE_HEIGHT;
		rectTile.right = rectTile.left + TILE_WIDTH;
		rectTile.bottom = rectTile.top + TILE_HEIGHT;
		int x, y;
		x = i * TILE_WIDTH;
		y = this->offset_y;
		CTile* tile = new CTile(x, y, rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);
		tiledMap.push_back(tile);
	}
	this->offset_y += TILE_HEIGHT;
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	HUD = Board::GetInstance();

	ifstream f;
	f.open(sceneFilePath);
	
	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }		
		if (line == "[MAP_INFO]") {
			section = SCENE_SECTION_MAP_INFO; continue;}
		if (line == "[TILE_MAP]") {
			section = SCENE_SECTION_TILE_MAP; continue;}
		
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_MAP_INFO: _ParseSection_MAP_INFO(line); break;
			case SCENE_SECTION_TILE_MAP:_ParseSection_TILE_MAP(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"resources\\bbox.png", D3DCOLOR_XRGB(0, 0, 0));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

}


void CPlayScene::Update(DWORD dt)
{	
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		if (objects[i]->visible == false)
			continue;
		coObjects.push_back(objects[i]);
	}

	/*for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->visible == false)
			continue;
		objects[i]->Update(dt, &coObjects);
	}*/

	// Get the bounding box of the viewport
	float left, top, right, bottom;
	CGame::GetInstance()->GetCameraBoundingBox(left, top, right, bottom);

	// Get objects in cells
	updateObjects.clear();
	grid = new CGrid();
	grid->Init(&coObjects,mapWidth, mapHeight, CELL_WIDTH, CELL_HEIGHT);

	//Get collidable objects
	grid->GetObjectsInRectangle(left, top, right, bottom, updateObjects);

	// Get collide-able objects
	for (UINT i = 0; i < updateObjects.size(); i++)
	{
		//if (updateObjects[i]->isVisible() == true)
			coObjects.push_back(updateObjects[i]);
	}

	// Update objects
	for (UINT i = 0; i < updateObjects.size(); i++)
	{
		//if (updateObjects[i]->isVisible() == true)
		
			updateObjects[i]->Update(dt, &coObjects);
		
	}

	// skip the rest if scene was already unloaded (Simon::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow simon
	float cx, cy;
	player->GetPosition(cx, cy);	
	CGame* game = CGame::GetInstance();

	if (cx >= SCREEN_WIDTH/ 2)
	{
		cx -= game->GetScreenWidth() / 2;
		cy -= game->GetScreenHeight() / 2;

		if (cx > mapWidth - SCREEN_WIDTH )
		{
			cx = mapWidth - SCREEN_WIDTH;
		}
	}

	else if (cx < SCREEN_WIDTH / 2)
	{
		cx = 0.0f;
	}
	
	// CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
	game ->SetCamPos(cx, 0.0f /*cy*/);

	HUD->Update(dt);
}

void CPlayScene::Render()
{
	// Render map
	for (int i = 0; i < tiledMap.size(); i++)
		tiledMap[i]->Render();

	for (int i = objects.size()-1; i >=0;i--) // Simon is rendered at the last 
	{
		if (objects[i]->visible == false)
			continue;
		objects[i]->Render();
	}	

	HUD->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
	{		
		if (dynamic_cast<CSimon*>(objects[i]) || 
			dynamic_cast<CWhip*>(objects[i]) || 
			dynamic_cast<CDagger*>(objects[i]) ||
			dynamic_cast<CBoomerang*>(objects[i]))
		{
			;
		}
		else
		{
			delete objects[i];
		} 
	}

	objects.clear();
	tiledMap.clear();
	CItems::GetInstance()->Clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);	
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"KeyDown: %d\n", KeyCode);

	CSimon *simon = ((CPlayScene*)scence)->GetPlayer();
	CDagger *dagger = ((CPlayScene*)scence)->GetDagger();
	CBoomerang *boomerang = ((CPlayScene*)scence)->GetBoomerang();

	switch (KeyCode)
	{	
	case DIK_SPACE:
	{
		if (simon->GetState() == SIMON_STATE_JUMP||
			simon->GetState() == SIMON_STATE_ATTACK || 
			simon->GetState() == SIMON_STATE_SIT_ATTACK ||
			simon->onStairs !=0)
			return;

		simon->SetState(SIMON_STATE_JUMP);
		break;
	}		
	case DIK_S: // Attack
	{

		if (CGame::GetInstance()->IsKeyDown(DIK_UP)) // Sub weapon attack 
		{
			if (simon->subWeapon == false)
				return;
			if (simon->GetState()== SIMON_STATE_THROW && dagger->visible == true) return;

			// UseDagger();
			float xS, yS;
			simon->GetPosition(xS, yS);
			dagger->SetPosition(xS, yS);
			dagger->SetOrientation(simon->nx);
			dagger->SetVisible(true);
			simon->SetState(SIMON_STATE_THROW);
		}

		// If Simon's state attack is not end, then continue
		if ((simon->GetState() == SIMON_STATE_ATTACK ||
			simon->GetState() == SIMON_STATE_SIT_ATTACK))
			return;

		if (simon->GetState() == SIMON_STATE_IDLE ||
			simon->GetState() == SIMON_STATE_JUMP ||
			simon->GetState() == SIMON_STATE_GO_UPSTAIR||
			simon->GetState()== SIMON_ANI_GO_DOWNSTAIR) 
		{
			simon->SetState(SIMON_STATE_ATTACK);
		}
		else if (simon->GetState() == SIMON_STATE_SIT)
		{
			simon->SetState(SIMON_STATE_SIT_ATTACK);
		}
		break;
	}	

	// Testing Boomerang function
	case DIK_D:
	{
		if (simon->subWeapon == false)
			return;
		if (simon->GetState() == SIMON_STATE_THROW && boomerang->visible == true) return;

		// UseBoomerang();
		float xS, yS;
		simon->GetPosition(xS, yS);
		boomerang->SetPosition(xS, yS);
		boomerang->SetOrientation(simon->nx);
		boomerang->SetVisible(true);
		simon->SetState(SIMON_STATE_THROW);
		break;
		
	}
	
	case DIK_Q: // Upgrade whip
	{
		simon->whip->PowerUp();
		// simon->nextSceneWhip->PowerUp();
		break;
	}

	//case DIK_A: // reset
	//	simon->Reset(); 
	//	break;

	case DIK_1:
		CGame::GetInstance()->SwitchScene(1);
		break;

	case DIK_2:
		CGame::GetInstance()->SwitchScene(2);
		break;

	case DIK_3:
		CGame::GetInstance()->SwitchScene(3);
		break;

	case DIK_4:
		CGame::GetInstance()->SwitchScene(4);
		break;

	case DIK_5:
		CGame::GetInstance()->SwitchScene(5);
		break;

	case DIK_6:
		CGame::GetInstance()->SwitchScene(6);
		break;

	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{}


void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CSimon *simon = ((CPlayScene*)scence)->GetPlayer();


	// When Simon is not touched on the ground, continue rendering jump animation
	if (simon->GetState() == SIMON_STATE_JUMP && simon->isOnGround() == false)		
		return;

	// Condition to stopping Simon's attacking loop
	if (simon->GetState() == SIMON_STATE_ATTACK &&
		simon->animation_set->at(SIMON_ANI_ATTACK)->IsOver(SIMON_ATTACK_TIME) == false) return;
	
		//simon->animation_set->at(SIMON_ANI_ATTACK)->Reset();
		

	if (simon->GetState() == SIMON_STATE_SIT_ATTACK && 
		simon->animation_set->at(SIMON_ANI_SIT_ATTACK)->IsOver(SIMON_ATTACK_TIME) == false)
		return;

	if (simon->GetState() == SIMON_STATE_THROW &&
		simon->animation_set->at(SIMON_ANI_THROW)->IsOver(SIMON_ATTACK_TIME) == false)
		return;

	if (simon->GetState() == SIMON_STATE_ATTACK &&
		simon->animation_set->at(SIMON_ANI_ATTACK_UPSTAIR)->IsOver(SIMON_ATTACK_TIME) == false)
		return;

	if (simon->GetState() == SIMON_STATE_ATTACK &&
		simon->animation_set->at(SIMON_ANI_ATTACK_DOWNSTAIR)->IsOver(SIMON_ATTACK_TIME) == false)
		return;

	if (simon->GetState() == SIMON_STATE_DEFLECT && 
		simon->animation_set->at(SIMON_ANI_DEFLECT)->IsOver(SIMON_DEFLECT_TIME) == false)
		return ;
	
	if (simon->powerUp == true) return;
	
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (simon->onStairs == 0)
		{
			simon->SetOrientation(1);
			simon->SetState(SIMON_STATE_WALKING);
		}
		else
		{
			simon->SetOrientation(1);
			simon->SetState(SIMON_STATE_GO_UPSTAIR);
		}
		
	}

	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (simon->onStairs == 0)
		{
			simon->SetOrientation(-1);
			simon->SetState(SIMON_STATE_WALKING);
		}
		else
		{
			simon->SetState(SIMON_STATE_GO_DOWNSTAIR);						
		}
	}

	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (simon->onStairs == 0&& simon->ovObjects.size()==0 )
			simon->SetState(SIMON_STATE_SIT);
		else
			simon->SetState(SIMON_STATE_GO_DOWNSTAIR);
	}

	else if (game->IsKeyDown(DIK_UP))
		simon->SetState(SIMON_STATE_GO_UPSTAIR);

	else
		simon->SetState(SIMON_STATE_IDLE);
}