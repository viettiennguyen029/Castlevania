#pragma once
#include "GameObject.h"
#include <unordered_map>

using namespace std;
/*
Manage items database
*/
class CItems
{
	unordered_map<int, vector<LPGAMEOBJECT>> items;
	static CItems* __instance;

public:
	void AddItem(int itemType, LPGAMEOBJECT item);
	void CheckAndDrop(LPGAMEOBJECT Object);			// Check if the given object is holding item, drop it if yes
	void Drop(int itemType, float x, float y);
	CItems() { };
	static CItems* GetInstance();
};