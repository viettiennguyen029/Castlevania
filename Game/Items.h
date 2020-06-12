#pragma once
#include "GameObject.h"
#include <unordered_map>
#define ITEM_LIFESPAN	5000

using namespace std;
/*
Manage items database
*/
class CItems
{
	unordered_map<int, vector<LPGAMEOBJECT>> items;
	static CItems* __instance;
	CItems() = default;
public:
	void AddItem(int itemType, LPGAMEOBJECT item);
	void CheckAndDrop(LPGAMEOBJECT Object);			// Check if the given object is holding item, drop it if yes
	void Drop(int itemType, float x, float y);
	void Clear();
	//CItems() { };
	static CItems* GetInstance();
};