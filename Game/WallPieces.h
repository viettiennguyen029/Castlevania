#pragma once
#include "GameObject.h"
using namespace std;

class CWallPiece : public CGameObject
{
public:
	CWallPiece();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL, bool stopMoving = false);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
}; 

/*
Manage WallPices group
*/
class CWallPieces
{
public:
	vector <CWallPiece*> pieces;
	static CWallPieces* __instance;

	void AddPiece(CWallPiece* piece);
	void DropPiece(float x, float y);
	void Clear();
	CWallPieces() { };
	static CWallPieces* GetInstance();
};

