#pragma once
#include"Game.h"
#include"GameObject.h"
#include "Simon.h"
#include "Code.h"

#define DEFAULT_GAME_TIME	300

class Board
{
	//int score;
	int time;	

	static Board* __instance;

	Code* code;
	DWORD count;
	bool isStop= false;

	CSimon* simon = CSimon::GetInstance();
	LPGAMEOBJECT boss;
	//CBossBat *boss = CPhan

public: 
	Board();	
	void Render();
	//void RenderBossHP(float x, float y, int bossHP);
	void GetBoss(LPGAMEOBJECT _boss) { this->boss = _boss; }
	void Update(DWORD dt);
	void ResetTimer();
	static Board* GetInstance();
};
