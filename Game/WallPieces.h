#pragma once
#include "GameObject.h"
using namespace std;
class CWallPiece : public CGameObject
{
public:
	CWallPiece();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}	
}; typedef CWallPiece* LPWALLPIECE;


/*
Manage WallPices group
*/
class CWallPieces
{
public:
	vector <LPWALLPIECE> pieces;
	static CWallPieces* __instance;

	void AddPiece(LPWALLPIECE piece);
	void DropPiece(LPGAMEOBJECT Object);
	CWallPieces() { };
	static CWallPieces* GetInstance();
};

