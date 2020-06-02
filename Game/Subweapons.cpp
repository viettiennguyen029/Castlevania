#include "SubWeapons.h"

SubWeapons* SubWeapons::__instance = NULL;
SubWeapons* SubWeapons::GetInstance()
{
	if (__instance == NULL)
		__instance = new SubWeapons();

	return __instance;
}

LPGAMEOBJECT SubWeapons::GetWeapon(SubWeapon weapon)
{
	vector<LPGAMEOBJECT> weaponName = subweapons[weapon];

	for (auto it = weaponName.begin(); it != weaponName.end(); ++it)
	{
		if ((*it)->isVisible() == false)
		{
			//(*quantityOfWeapon)--;
			return *it;
		}
	}

	return NULL;
}

void SubWeapons::Add(SubWeapon name, LPGAMEOBJECT weapon)
{
	weapon->SetVisible(true);
	subweapons[name].push_back(weapon);
}
