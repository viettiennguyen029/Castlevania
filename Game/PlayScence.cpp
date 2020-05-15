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
#define SCENE_SECTION_TEXTURES				2
#define SCENE_SECTION_SPRITES					3
#define SCENE_SECTION_ANIMATIONS			4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS					6
#define SCENE_SECTION_TILE_MAP				7

#define OBJECT_TYPE_SIMON						0
#define OBJECT_TYPE_BRICK						1
#define OBJECT_TYPE_CANDLE					2
#define OBJECT_TYPE_WHIP						3
#define OBJECT_TYPE_ITEM_BIG_HEART		4
#define OBJECT_TYPE_ITEM_CHAIN			5
#define OBJECT_TYPE_ITEM_DAGGER			6
#define OBJECT_TYPE_DAGGER					7
#define OBJECT_TYPE_BLACK_KNIGHT		8

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	if (texID == MAP_ID)
	{
		this->mapWidth = atoi(tokens[5].c_str());
		this->offset_y = atoi(tokens[6].c_str());
}

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));	
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

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

	switch (object_type)
	{	
	case OBJECT_TYPE_SIMON:
	{
		if (player != NULL)
		{
			DebugOut(L"[ERROR] SIMON object was created before! ");
			return;
		}
		obj = new CSimon(x,y);
		player = (CSimon*)obj;
		DebugOut(L"[INFO] Player object created!\n");
		break;
	}

	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_WHIP: obj = new CWhip(); break;
	case OBJECT_TYPE_DAGGER: obj = new CDagger(); break;
	case OBJECT_TYPE_BLACK_KNIGHT: obj = new CBlack_Knight(); break;

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
		CItems::GetInstance()->AddItem((int)CGameObject::ItemType::BIG_HEART, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_CHAIN:
	{
		 obj = new ItemChain();
		 CItems::GetInstance()->AddItem((int)CGameObject::ItemType::CHAIN, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_DAGGER:
	{
		obj = new ItemDagger();
		CItems::GetInstance()->AddItem((int)CGameObject::ItemType::DAGGER, obj);
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

void CPlayScene::_ParseSection_TILE_MAP(string line)
{
	LPDIRECT3DTEXTURE9 tilesheet = CTextures::GetInstance()->Get(MAP_ID);
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

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[TILE_MAP]") {
			section = SCENE_SECTION_TILE_MAP; continue;}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
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

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->visible == false)
			continue;
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Simon::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow simon
	float cx, cy;
	player->GetPosition(cx, cy);

	if ( cx> mapWidth -SCREEN_WIDTH/2)
	{
		return;
	}

	CGame *game = CGame::GetInstance();
	cx -= game->GetScreenWidth()/2 ;
	cy -= game->GetScreenHeight()/2;

	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
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
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
		
	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
	int currentMapId = CGame::GetInstance()->GetCurrentScene()->GetSceneId() * 100;
	// CMaps::GetInstance()->Get(currentMapId)->
}


void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"KeyDown: %d\n", KeyCode);

	CSimon *simon = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{	
	case DIK_SPACE:
	{
		if (simon->GetState() == SIMON_STATE_JUMP||
			simon->GetState() == SIMON_STATE_ATTACK || 
			simon->GetState() == SIMON_STATE_SIT_ATTACK)
			return;

		simon->SetState(SIMON_STATE_JUMP);
		break;
	}		
	case DIK_S: // Attack
		// If Simon's state attack is not end, then continue
		if ((simon->GetState() == SIMON_STATE_ATTACK || 
			simon->GetState() == SIMON_STATE_SIT_ATTACK))
			return;

		if (simon->GetState() == SIMON_STATE_IDLE || 
			simon->GetState() == SIMON_STATE_JUMP) // Đứng đánh
		{
			simon->SetState(SIMON_STATE_ATTACK);
		}
		else if (simon->GetState() == SIMON_STATE_SIT) // Ngồi đánh
		{
			simon->SetState(SIMON_STATE_SIT_ATTACK);
		}
		break;

	case DIK_D: // Sub attack
	{
		break;
	}
	case DIK_Q: // Upgrade whip
	{
		simon->whip->PowerUp();
		break;
	}

	case DIK_A: // reset
		simon->Reset(); 
		break;

	case DIK_2:
		CGame::GetInstance()->SwitchScene(2);
		break;

	case DIK_3:
		CGame::GetInstance()->SwitchScene(3);
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
	if (simon->GetState()== SIMON_STATE_ATTACK && 
		simon->animation_set->at(SIMON_ANI_ATTACK)->IsOver(SIMON_ATTACK_TIME) == false)
		return;

	if (simon->GetState() == SIMON_STATE_SIT_ATTACK && 
		simon->animation_set->at(SIMON_ANI_SIT_ATTACK)->IsOver(SIMON_ATTACK_TIME) == false)
		return;
    			
	if (game->IsKeyDown(DIK_RIGHT))
	{
		simon->SetOrientation(1);
		simon->SetState(SIMON_STATE_WALKING);		
	}	

	else if (game->IsKeyDown(DIK_LEFT))
	{
		simon->SetOrientation(-1);
		simon->SetState(SIMON_STATE_WALKING);
	}	

	else if (game->IsKeyDown(DIK_DOWN))
		simon->SetState(SIMON_STATE_SIT);

	else if (game->IsKeyDown(DIK_UP))
		simon->SetState(SIMON_STATE_GO_UPSTAIR);

	else
		simon->SetState(SIMON_STATE_IDLE);
}