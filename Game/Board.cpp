#include "Board.h"

Board* Board::__instance = NULL;
void Board::Render()
{
	CSprites* sprites = CSprites::GetInstance();	

	CGame* game = CGame::GetInstance();
	float cx, cy;
	game->GetCamPos(cx, cy);

	// Game info
	sprites->Get(80000)->Draw(cx , cy+2,-1);

	code->DrawNumber(4, cx+150, cy +8, time); // Play time
	code->DrawNumber(6, cx + 55, cy + 8, 123456);  // Player score
	code->DrawNumber(2, cx + 234, cy + 8, game->GetSceneId());  // Curent Scene

	code->DrawNumber(2, cx + 200, cy + 18, simon->GetHeartQuantity()); // Heart quantity
	code->DrawNumber(2, cx + 200, cy + 28, 03); // Current Life's Points

	// Sub-weapon
	code->DrawSubWeapon(cx + 158, cy + 17, simon->GetSubWeapon());

	// HP
	code->DrawHP(cx + 60, cy + 18, 0, simon->GetHP()); // Player's HP
	code->DrawHP(cx + 60, cy + 28, 1, 16); // Boss's HP
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
