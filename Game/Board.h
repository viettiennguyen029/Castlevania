#pragma once
#include"Game.h"
#include"GameObject.h"
#include "Simon.h"
#include "Code.h"

class Board
{
	//int score;
	int time;	

	static Board* __instance;

	Code* code;
	DWORD count;
	bool isStop= false;

	CSimon* simon = CSimon::GetInstance();
public: 
	Board();	
	void Render();
	void Update(DWORD dt);
	static Board* GetInstance();
};

