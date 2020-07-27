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
#define SCENE_SECTION_PLAYER					0
#define SCENE_SECTION_TEXTURES				2
#define SCENE_SECTION_SPRITES					3
#define SCENE_SECTION_ANIMATIONS			4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS					6
#define SCENE_SECTION_MAP_INFO				7
#define SCENE_SECTION_TILE_MAP				8

#define SCENE_SECTION_RELEASE_TEXTURES		-2
#define SCENE_SECTION_RELEASE_SPRITES			-3
#define SCENE_SECTION_RELEASE_ANIMATIONS	-4
#define SCENE_SECTION_RELEASE_ANIMATION_SETS	-5

#define OBJECT_TYPE_SIMON						0
#define OBJECT_TYPE_BRICK						1
#define OBJECT_TYPE_CANDLE					2

#define OBJECT_TYPE_ITEM_BIG_HEART			4
#define OBJECT_TYPE_ITEM_SMALL_HEART	44
#define OBJECT_TYPE_ITEM_CHAIN				5

#define OBJECT_TYPE_ITEM_MONEY_BAG_RED			10
#define OBJECT_TYPE_ITEM_MONEY_BAG_PURPLE	11
#define OBJECT_TYPE_ITEM_MONEY_BAG_YELLOW	12

#define OBJECT_TYPE_ITEM_DAGGER				80
#define OBJECT_TYPE_ITEM_BOOMERANG	81
#define OBJECT_TYPE_ITEM_HOLY_WATER		82
#define OBJECT_TYPE_ITEM_INVISIBILITY		83
#define OBJECT_TYPE_ITEM_AXE					84
#define OBJECT_TYPE_ITEM_MEAT					85
#define OBJECT_TYPE_ITEM_WATCH				86

#define OBJECT_TYPE_DAGGER					7
#define OBJECT_TYPE_BOOMERANG			71
#define OBJECT_TYPE_HOLY_WATER			72
#define OBJECT_TYPE_AXE							73

#define OBJECT_TYPE_ZOMBIE					63
#define OBJECT_TYPE_HUNCH_BACK			64
#define OBJECT_TYPE_SKELETON				65
#define OBJECT_TYPE_GHOST					66
#define  OBJECT_TYPE_PHANTOM_BAT		67
#define OBJECT_TYPE_RAVEN						68
#define OBJECT_TYPE_BONES						69

#define OBJECT_TYPE_BLACK_KNIGHT		8
#define OBJECT_TYPE_BAT							9

#define OBJECT_TYPE_VARIOUS_STAIR	-1
#define OBJECT_TYPE_STAIR_BOTTOM	-2
#define OBJECT_TYPE_STAIR_TOP			-3

#define OBJECT_TYPE_MOVING_PLATFORM	30
#define OBJECT_TYPE_CROWN_ITEM				40
#define OBJECT_TYPE_PORTAL						50
#define OBJECT_TYPE_DOOR							51
#define OBJECT_TYPE_BREAK_WALL				90
#define OBJECT_TYPE_WALL_PIECES				91
#define OBJECT_TYPE_POINT_EFFECTS			92

#define MAX_SCENE_LINE 1024
#define STOP_WATCH_IDLE_TIME	3000

void CPlayScene::_ParseSection_SCENE_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SCENE_SPRITES(string line)
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

void CPlayScene::_ParseSection_SCENE_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_SCENE_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void CPlayScene::_ParseSection_SCENE_PLAYER(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* playerObj = NULL;

	if (player != NULL)
	{
		DebugOut(L"[ERROR] SIMON object was created before! ");
		return;
	}


	playerObj = CSimon::GetInstance();
	player = (CSimon*)playerObj;

	playerObj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	playerObj->SetAnimationSet(ani_set);

	DebugOut(L"[INFO] Player object created!\n");
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_SCENE_OBJECTS(string line)
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
		obj->SetVisible(false);
		CSubWeapon::GetInstance()->Add((int)SubWeapon::DAGGER, obj);
		break;
	}

	case OBJECT_TYPE_BOOMERANG:
	{
		obj = new CBoomerang();
		obj->SetVisible(false); 
		CSubWeapon::GetInstance()->Add((int)SubWeapon::BOOMERANG, obj);
		break;
	}

	case OBJECT_TYPE_HOLY_WATER:
	{
		obj = new CHolyWater();
		CSubWeapon::GetInstance()->Add((int)SubWeapon::HOLYWATER, obj);
		break;
	}

	case OBJECT_TYPE_AXE:
	{
		obj = new CAxe();
		CSubWeapon::GetInstance()->Add((int)SubWeapon::AXE, obj);
		break;
	}

	case OBJECT_TYPE_MOVING_PLATFORM: obj = new CMovingPlatform(); break;

	case OBJECT_TYPE_CROWN_ITEM: obj = new ItemCrown(); break;

	case OBJECT_TYPE_BREAK_WALL:
	{
		int state = atoi(tokens[4].c_str());
		int it = atoi(tokens[5].c_str());
		obj = new CBreakWall(x, y); 
		obj->SetState(state);
		obj->SetItemId(it);
		break;
	}

	case OBJECT_TYPE_WALL_PIECES: 
	{
		obj = new CWallPiece();		
		CWallPieces::GetInstance()->AddPiece((CWallPiece*)obj);
		break;
	}

	case OBJECT_TYPE_POINT_EFFECTS:
	{
		obj = new PointEffect();
		PointEffects::GetInstance()->Add((PointEffect*)obj);
		break;
	}
	case OBJECT_TYPE_BAT:
	{
		int it = atoi(tokens[4].c_str());
		obj = new CBat(x,y);
		obj->SetItemId(it);
		break;
	}	
	case OBJECT_TYPE_BLACK_KNIGHT: 
	{
		int it = atoi(tokens[4].c_str());
		obj = new CBlack_Knight(x,y); 
		obj->SetItemId(it);
		break;
	}

	case OBJECT_TYPE_ZOMBIE:
	{
		obj = new CZombie();
		break;
	}

	case OBJECT_TYPE_HUNCH_BACK:
	{
		obj = new CHunchBack();
		break;
	}

	case OBJECT_TYPE_SKELETON:
	{
		obj = new CSkeleton();
		break;
	}

	case OBJECT_TYPE_RAVEN:
	{
		obj = new CRaven();
		obj->SetOrientation(-1);
		break;
	}

	/*case OBJECT_TYPE_BONES:
	{
		obj = new CBones();
		break;
	}*/
	case OBJECT_TYPE_GHOST:
	{
		obj = new CGhost();
		break;
	}

	case OBJECT_TYPE_PHANTOM_BAT:
	{
		obj = new CPhantomBat(x,y);
		boss = (CPhantomBat*)obj;
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
		obj->SetVisible(false);
		items->AddItem((int)ItemType::BIG_HEART, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_SMALL_HEART:
	{
		obj = new ItemSmallHeart ();
		obj->SetVisible(false);
		items->AddItem((int)ItemType::SMALL_HEART, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_CHAIN:
	{
		 obj = new ItemChain();
		 obj->SetVisible(false);
		 items->AddItem((int)ItemType::CHAIN, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_DAGGER:
	{
		obj = new ItemDagger();
		items->AddItem((int)ItemType::DAGGER, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_HOLY_WATER:
	{
		obj = new ItemHolyWater();
		obj->SetVisible(false);
		items->AddItem((int)ItemType::HOLY_WATER, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_INVISIBILITY:
	{
		obj = new ItemInvisibility();
		obj->SetVisible(false);
		items->AddItem((int)ItemType::INVISIBILITY, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_AXE:
	{
		obj = new ItemAxe();
		obj->SetVisible(false);
		items->AddItem((int)ItemType::AXE, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_MEAT:
	{
		obj = new ItemMeat();
		obj->SetVisible(false);
		items->AddItem((int)ItemType::MEAT, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_MONEY_BAG_RED:
	{
		obj = new ItemMoneyBagRed();
		obj->SetVisible(false);
		items->AddItem((int)ItemType::MONEY_BAG_RED, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_MONEY_BAG_PURPLE:
	{
		obj = new ItemMoneyBagPurple();
		obj->SetVisible(false);
		items->AddItem((int)ItemType::MONEY_BAG_PURPLE, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_MONEY_BAG_YELLOW:
	{
		obj = new ItemMoneyBagYellow();
		obj->SetVisible(false);
		items->AddItem((int)ItemType::MONEY_BAG_YELLOW, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_BOOMERANG:
	{
		obj = new ItemBoomerang();
		obj->SetVisible(false);
		items->AddItem((int)ItemType::BOOMERANG, obj);
		break;
	}

	case OBJECT_TYPE_ITEM_WATCH:
	{
		obj = new ItemWatch();
		obj->SetVisible(false);
		items->AddItem((int)ItemType::STOP_WATCH, obj);
		break;
	}

	case OBJECT_TYPE_VARIOUS_STAIR:
	{
		int nx = atoi(tokens[4].c_str());
		obj = new CVariousStair();		
		obj->SetOrientation(nx);
		break;
	}

	case OBJECT_TYPE_STAIR_BOTTOM:
	{
		int nx = atoi(tokens[4].c_str());		
		obj = new CStairBottom();
		obj->SetOrientation(nx);
		break;
	}

	case OBJECT_TYPE_STAIR_TOP:
	{
		int nx = atoi(tokens[4].c_str());
		obj = new CStairTop();	
		obj->SetOrientation(nx);
		break;
	}

	case OBJECT_TYPE_DOOR:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		obj = new CDoor(x, y, r, b);
		break;
	}

	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
		break;
	}

	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);	
	objects.push_back(obj);

	if (obj->isVisible() == false)
		invisibleObjects.push_back(obj);

	grid ->Classify(obj);
	
}

void CPlayScene::_ParseSection_SCENE_MAP_INFO(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 3) return; // skip invalid lines
	this->mapWidth = atoi(tokens[0].c_str());
	this->mapHeight = atoi(tokens[1].c_str());
	this->offset_y= atoi(tokens[2].c_str());
}

void CPlayScene::_ParseSection_SCENE_TILE_MAP(string line)
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

//void CPlayScene::_ReleaseSection_SCENE_TEXTURES(string line)
//{
//	vector<string> tokens = split(line);	
//	
//	for (int i = 0; i < tokens.size(); i++)
//	{
//		int texID = atoi(tokens[i].c_str());
//		CTextures::GetInstance()->Release(texID);
//	}		
//}

//void CPlayScene::_ReleaseSection_SCENE_SPRITES(string line)
//{
//	vector<string> tokens = split(line);
//
//	for (int i = 0; i < tokens.size(); i++)
//	{
//		int spriteID = atoi(tokens[i].c_str());
//		CSprites::GetInstance()->Release(spriteID);
//	}
//}

//void CPlayScene::_ReleaseSection_SCENE_ANIMATIONS(string line)
//{
//	vector<string> tokens = split(line);
//
//	for (int i = 0; i < tokens.size(); i++)
//	{
//		int aniID = atoi(tokens[i].c_str());
//		CAnimations::GetInstance()->Release(aniID);
//	}
//}

//void CPlayScene::_ReleaseSection_SCENE_ANIMATION_SETS(string line)
//{
//	vector<string> tokens = split(line);
//
//	for (int i = 0; i < tokens.size(); i++)
//	{
//		int ani_set_id = atoi(tokens[i].c_str());
//		CAnimationSets::GetInstance()->Release(ani_set_id);
//	}
//}


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

		if (line == "[PLAYER]") {
			section = SCENE_SECTION_PLAYER; continue;
		}
	
		if (line == "[TEXTURES]"){
			section = SCENE_SECTION_TEXTURES; continue;
		}

		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}

		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATIONS_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}

		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; 
		}		

		if (line == "[MAP_INFO]") {
			section = SCENE_SECTION_MAP_INFO; continue;
		}
		if (line == "[TILE_MAP]") {
			section = SCENE_SECTION_TILE_MAP; continue;
		}
		
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_SCENE_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SCENE_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_SCENE_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_SCENE_ANIMATION_SETS(line); break;
			case SCENE_SECTION_PLAYER:_ParseSection_SCENE_PLAYER(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_SCENE_OBJECTS(line); break;
			case SCENE_SECTION_MAP_INFO: _ParseSection_SCENE_MAP_INFO(line); break;
			case SCENE_SECTION_TILE_MAP:_ParseSection_SCENE_TILE_MAP(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"resources\\bbox.png", D3DCOLOR_XRGB(0, 0, 0));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}


void CPlayScene::Update(DWORD dt)
{	
	// skip the rest if scene was already unloaded (Simon::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// We know that Simon is the first object in the list hence we won't add him into the colliable object list
	vector<LPGAMEOBJECT> coObjects;
	
	/*for (size_t i = 0; i < objects.size(); i++)
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
	}*/
	
	// Update camera to follow simon
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();

	if (boss !=NULL)
	{
		if (boss->inActive == false) // Lock cam
		{
			cx = mapWidth - SCREEN_WIDTH;
			game->SetCamPos(cx+10.0f , 0.0f);
		}
		else
		{
			cx -= game->GetScreenWidth() / 2;
			game->SetCamPos(cx+10.0f, 0.0f);
		}
		
			
	}
	else
	{
		cx -= game->GetScreenWidth() / 2;
		if (cx >= (mapWidth - SCREEN_WIDTH))
			cx = mapWidth - SCREEN_WIDTH;

		game->SetCamPos(cx + 10, 0.0f /*cy*/);
	}
	

	// Get the bounding box of the viewport
	float left, top, right, bottom;
	game->GetCameraBoundingBox(left, top, right, bottom);

	updateObject.clear();
	coObjects.clear();

	//Get objects in grid
	grid->GetObjectsInGrid(updateObject, left, top, right, bottom);

	//hiddenObject.clear();
	for (size_t i = 0; i < invisibleObjects.size(); i++)
	{
		if (invisibleObjects[i]->isVisible())
		{
			// Condition to prevent adding object repeatedly
			if (find(updateObject.begin(), updateObject.end(), invisibleObjects[i]) != updateObject.end() == false)
			{
				updateObject.push_back(invisibleObjects[i]);
			}
		}
	}

	// Get collide-able objects in the grid 
	for (size_t i = 0; i < updateObject.size(); i++)
	{
		if (updateObject[i]->isVisible() == true)
		{
			if (CSkeleton *skt =dynamic_cast<CSkeleton*>(updateObject[i]))
			{
				coObjects.push_back(skt);
				vector<LPGAMEOBJECT> bones = skt->GetBones();

				// Adding bones to coObjects
				for (UINT i = 0; i < skt->GetBones().size(); i++)
				{	
					coObjects.push_back(bones[i]);
				}	
			}
			else 
				coObjects.push_back(updateObject[i]);
		}			
	}
	//DebugOut(L"CoObject: %d, Object update: %d\n", coObjects.size(), updateObject.size());

	// Call Update function of each object
	for (size_t i = 0; i < updateObject.size(); i++)
	{
		if (updateObject[i]->isVisible() == true)
			updateObject[i]->Update(dt, &coObjects, stopMoving);				
	}

	player->Update(dt, &coObjects);
	

	// check if Simon is using stop watch
	if (CSimon::GetInstance()->StopWatch())
	{
		if (stopMoving_start < STOP_WATCH_IDLE_TIME)
		{
			stopMoving = true;
			stopMoving_start += dt;
		}
		else
		{
			stopMoving = false;
			stopMoving_start = 0;
			player->SetStopWatch(false);
			player->SetState(SIMON_STATE_IDLE);
		}
	}
	else
		stopMoving = false;

	
	HUD->Update(dt);

}

void CPlayScene::Render()
{
	// Render map
	for (int i = 0; i < tiledMap.size(); i++)
		tiledMap[i]->Render();

	for (int i = objects.size() - 1; i >= 0; i--)
	{
		if (objects[i]->visible == false)
			continue;
		objects[i]->Render();
	}	

	player->Render();// Simon is rendered at the last 
	HUD->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
#pragma region Release Resources
	//ifstream f;
	//f.open(releaseScene);

	//// current resource section flag
	//int section = SCENE_SECTION_UNKNOWN;

	//char str[MAX_SCENE_LINE];
	//while (f.getline(str, MAX_SCENE_LINE))
	//{
	//	string line(str);

	//	if (line[0] == '#') continue;	// skip comment lines	

	//	if (line == "[RELEASE_TEXTURES]") {
	//		section = SCENE_SECTION_RELEASE_TEXTURES; continue;
	//	}

	//	if (line == "[RELEASE_SPRITES]") {
	//		section = SCENE_SECTION_RELEASE_SPRITES; continue;
	//	}

	//	if (line == "[RELEASE_ANIMATIONS]") {
	//		section = SCENE_SECTION_RELEASE_ANIMATIONS; continue;
	//	}
	//	if (line == "[RELEASE_ANIMATION_SETS]") {
	//		section = SCENE_SECTION_RELEASE_ANIMATION_SETS; continue;
	//	}

	//	if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

	//	switch (section)
	//	{
	//		case SCENE_SECTION_RELEASE_TEXTURES: _ReleaseSection_SCENE_TEXTURES(line); break;
	//		case SCENE_SECTION_RELEASE_SPRITES: _ReleaseSection_SCENE_SPRITES(line); break;
	//		case SCENE_SECTION_RELEASE_ANIMATIONS: _ReleaseSection_SCENE_ANIMATIONS(line); break;
	//		case SCENE_SECTION_RELEASE_ANIMATION_SETS: _ReleaseSection_SCENE_ANIMATION_SETS(line); break;
	//	}
	//}

	//f.close();
#pragma endregion
	for (int i = 0; i < objects.size(); i++)
	{
		if (/*dynamic_cast<CSimon*>(objects[i]) ||*/
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

	grid->Clear();

	updateObject.clear();
	objects.clear();
	tiledMap.clear();
	
	CItems::GetInstance()->Clear();
	CWallPieces::GetInstance()->Clear();
	PointEffects::GetInstance()->Clear();

	player = NULL;
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);	
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
			simon->GetState() == SIMON_STATE_SIT_ATTACK ||
			simon->onStairs !=0)
			return;

		simon->SetState(SIMON_STATE_JUMP);
		break;
	}		
	case DIK_S: // Attack
	{
		// If Simon's state attack is not end, then continue
		if ((simon->GetState() == SIMON_STATE_ATTACK ||simon->GetState() == SIMON_STATE_SIT_ATTACK))
			return;

		if (simon->GetState() == SIMON_STATE_IDLE ||	simon->GetState() == SIMON_STATE_JUMP ||
			simon->GetState() == SIMON_STATE_GO_UPSTAIR || simon->GetState() == SIMON_ANI_GO_DOWNSTAIR)
		{
			simon->SetState(SIMON_STATE_ATTACK);
		}

		else if (simon->GetState() == SIMON_STATE_SIT)
		{
			simon->SetState(SIMON_STATE_SIT_ATTACK);
		}

		if (CGame::GetInstance()->IsKeyDown(DIK_UP) ) // Sub weapon attack 
		{
			if (simon->GetSubWeapon() != (int)SubWeapon::UNKNOWN)
			{
				simon->SetState(SIMON_STATE_THROW);
			}			
		}		
		break;
	}

		
	case DIK_Q: // Use Dagger
	{
		simon->SetSubWeapon(int(SubWeapon::DAGGER));
		break;
	}

	case DIK_W: // Use Boomerang
	{
		simon->SetSubWeapon(int(SubWeapon::BOOMERANG));
		break;
	}

	case DIK_E: // Use Holy water
	{
		simon->SetSubWeapon(int(SubWeapon::HOLYWATER));
		break;
	}

	case DIK_R: // Use Axe
	{
		simon->SetSubWeapon(int(SubWeapon::AXE));
		break;
	}

	case DIK_T: // Use Stop Watch
	{
		simon->SetSubWeapon(int(SubWeapon::STOP_WATCH));
		break;
	}

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


	if (simon == NULL) return;

	// When Simon is not touched on the ground, continue rendering jump animation
	if (simon->GetState() == SIMON_STATE_JUMP && simon->isOnGround() == false)		
		return;
	
	// Condition to stopping Simon's attacking loop
	if (simon->GetState() == SIMON_STATE_ATTACK &&
		simon->animation_set->at(SIMON_ANI_ATTACK)->IsOver(SIMON_ATTACK_TIME) == false) return;
	
	if (simon->GetState() == SIMON_STATE_SIT_ATTACK && 
		simon->animation_set->at(SIMON_ANI_SIT_ATTACK)->IsOver(SIMON_ATTACK_TIME) == false)
		return;

	if (simon->GetState() == SIMON_STATE_THROW &&
		simon->animation_set->at(SIMON_ANI_ATTACK)->IsOver(SIMON_ATTACK_TIME) == false)
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
	
	if (simon->PowingUp() == true) return;
	
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (simon->onStairs == 0 )
		{
			simon->SetOrientation(1);
			if (simon->GetState() != SIMON_STATE_SIT) 
				simon->SetState(SIMON_STATE_WALKING);
			
		}
		else
		{
			if (simon->onStairs == 1)
				if (simon->nx == 1)
					simon->SetState(SIMON_STATE_GO_UPSTAIR);
				else
					simon->SetState(SIMON_STATE_GO_DOWNSTAIR);

			else if (simon->onStairs == -1)
				if (simon->nx == 1)
					simon->SetState(SIMON_STATE_GO_DOWNSTAIR);
				else
					simon->SetState(SIMON_STATE_GO_UPSTAIR);
		}
		
	}

	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (simon->onStairs == 0)
		{
			simon->SetOrientation(-1);
			if (simon->GetState() != SIMON_STATE_SIT)
				simon->SetState(SIMON_STATE_WALKING);
			//simon->SetState(SIMON_STATE_WALKING);
		}
		else
		{
			if (simon->onStairs == 1)
				if (simon->nx == 1)
					simon->SetState(SIMON_STATE_GO_DOWNSTAIR);
				else
					simon->SetState(SIMON_STATE_GO_UPSTAIR);

			else if (simon->onStairs == -1)
				if (simon->nx == 1)
					simon->SetState(SIMON_STATE_GO_UPSTAIR);
				else
					simon->SetState(SIMON_STATE_GO_DOWNSTAIR);
		}
	}

	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (simon->onStairs == 0 && simon->GetOverlapObjectSize() ==0)
			simon->SetState(SIMON_STATE_SIT);
		else
			simon->SetState(SIMON_STATE_GO_DOWNSTAIR);
	}

	else if (game->IsKeyDown(DIK_UP))
		simon->SetState(SIMON_STATE_GO_UPSTAIR);

	else
		simon->SetState(SIMON_STATE_IDLE);
}