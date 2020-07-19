#pragma once
#include<string>
using namespace std;

class Code
{	
public:
	void DrawSubWeapon(float x, float y, int subweapon);
	void DrawNumber(int max, float x, float y, int s);
	//void DrawHeartQuantity(float x, float y, int heart_quantity);
	void DrawHP(float x, float y, int type, int s);
	int Convert(char c);
};

