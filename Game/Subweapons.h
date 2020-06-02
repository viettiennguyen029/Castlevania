#pragma once
#include <unordered_map>
#include "Dagger.h"
#include "GameObject.h"

using namespace std;
class SubWeapons
{
	unordered_map<SubWeapon, vector<LPGAMEOBJECT>> subweapons;
	static SubWeapons* __instance;
public: 
	static SubWeapons* GetInstance();

	LPGAMEOBJECT GetWeapon(SubWeapon weapon);
	void Add(SubWeapon name, LPGAMEOBJECT weapon);
};

