#include "Code.h"
#include "Sprites.h"
#include "SubWeapon.h"

#define DAGGER_SPRTITE_ID			40003
#define BOOMERANG_SPITE_ID		40005
#define HOLY_WATER_SPRTITE_ID	40007
#define AXE_SPRTITE_ID					40009
#define WATCH_SPRITE_ID				40011


void Code::DrawNumber(int max, float x, float y, int s)
{
	string str = to_string(s);

	for (int i = 0; i < max - str.size(); i++)
	{
		CSprites::GetInstance()->Get(88000)->Draw(x, y,-1);
		x += 7;
	}

	for (int i = 0; i < str.size(); i++)
	{
		CSprites::GetInstance()->Get(88000 + Convert(str[i]))->Draw(x, y,-1);
		x += 7;
	}
}

void Code::DrawHP(float x, float y, int type, int s)
{
	CSprites* sprites = CSprites::GetInstance();
	CSprite* sprite1 = sprites->Get(88102);
	CSprite* sprite2 = NULL;

	if (type == 0) // simon health point
		sprite2 = sprites->Get(88101);
	else // Enemy
		sprite2 = sprites->Get(88103);

	for (int i = 0; i < s; i++)
	{
		sprite2->Draw(x, y,-1);
		x += 5;
	}

	
	for (int i = 0; i <16 - s; i++)
	{
		sprite1->Draw(x, y,-1);
		x += 5;
	}
	sprites = NULL;
	sprite1 = NULL;
	sprite2 = NULL;

}

void Code::DrawSubWeapon(float x, float y, int subweapon)
{
	switch (subweapon)
	{
	
	case (int)SubWeapon::DAGGER:
	{
		CSprites::GetInstance()->Get(DAGGER_SPRTITE_ID)->Draw(x, y+4, -1);
		break;
	}
	case(int)SubWeapon::BOOMERANG:
	{
		CSprites::GetInstance()->Get(BOOMERANG_SPITE_ID)->Draw(x, y+1, -1);
		break;
	}	
	case (int)SubWeapon::HOLYWATER:
	{
		CSprites::GetInstance()->Get(HOLY_WATER_SPRTITE_ID)->Draw(x-3, y, 1);
		break;
	}

	case (int)SubWeapon::AXE:
	{
		CSprites::GetInstance()->Get(AXE_SPRTITE_ID)->Draw(x, y + 1, -1);
		break;
	}

	case (int)SubWeapon::STOP_WATCH:
	{
		CSprites::GetInstance()->Get(WATCH_SPRITE_ID)->Draw(x, y, -1);
		break;
	}
	default:
		break;
	}
}

int Code::Convert(char c)
{	
	return c - '0';	
}
