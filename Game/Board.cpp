#include "Board.h"

Board* Board::__instance = NULL;
void Board::Render()
{
	CSprites* sprites = CSprites::GetInstance();	

	CGame* game = CGame::GetInstance();
	float cx, cy;
	game->GetCamPos(cx, cy);

	sprites->Get(80000)->Draw(cx , cy+2,-1);
	code->DrawNumber(4, cx+150, cy +8, time);
	code->DrawNumber(6, cx + 55, cy + 8, 123456);
	code->DrawNumber(2, cx + 234, cy + 8, game->GetSceneId());

	code->DrawHP(cx + 60, cy + 18, 0, 14);
	code->DrawHP(cx + 60, cy + 28, 1, 13);
}
Board::Board()
{ 
	time = 300;
	count = GetTickCount();
	isStop = false;
	code = new Code();
}

Board* Board::GetInstance()
{
	if (__instance == NULL)
		__instance = new Board();

	return __instance;
}

void Board::Update(DWORD dt)
{
	if (isStop)
	{
		return;
	}
	if (GetTickCount() - count >= 1000)
	{
		time--;
		count = GetTickCount();
	}
}
