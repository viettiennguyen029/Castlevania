#include "Code.h"
#include "Sprites.h"

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
	CSprite* sprite1 = sprites->Get(88101);
	CSprite* sprite2 = NULL;

	if (type == 0) // simon health point
		sprite2 = sprites->Get(88102);
	else // Enemy
		sprite2 = sprites->Get(88103);

	for (int i = 0; i < s; i++)
	{
		sprite2->Draw(x, y,-1);
		x += 6;
	}

	for (int i = 0; i < 6 - s; i++)
	{
		sprite1->Draw(x, y,-1);
		x += 6;
	}
	sprites = NULL;
	sprite1 = NULL;
	sprite2 = NULL;

}

int Code::Convert(char c)
{	
	return c - '0';	
}
