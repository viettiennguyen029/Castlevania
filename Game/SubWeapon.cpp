#include "SubWeapon.h"
#include "Simon.h"

CSubWeapon* CSubWeapon::__instance = NULL;

void CSubWeapon::Add(int weapon, LPGAMEOBJECT obj)
{
	subweapons[weapon].push_back(obj);
}


void CSubWeapon::Select(int weapon)
{
	switch (weapon)
	{
	case (int)SubWeapon::DAGGER:
		UseDagger();
		break;

	case (int)SubWeapon::BOOMERANG:
		UseBoomerang();
		break;

	case (int)SubWeapon::HOLYWATER:
		UseHolyWater();
		break;

	default:
		break;
	}
}

void CSubWeapon::UseDagger()
{
}

void CSubWeapon::UseBoomerang()
{
}

void CSubWeapon::UseHolyWater()
{
	LPGAMEOBJECT holywater = GetWeapon((int)SubWeapon::HOLYWATER);
	if (holywater == NULL) return;
	holywater->SetVisible(true);
	holywater->nx = CSimon::GetInstance()->GetOrientation();
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);
	float xW = xS;
	float yW= yS + 5;
	holywater->SetPosition(xW, yW);
}

LPGAMEOBJECT CSubWeapon::GetWeapon(int weapon)
{
	if (subweapons[weapon].empty())
		DebugOut(L"\n[ERROR] No weapons");

	else
	{
		for (auto i = subweapons[weapon].begin(); i != subweapons[weapon].end(); ++i)
		{
			if ((*i)->isVisible() == false)
			{
				return (*i);
				break;
			}
		}
	}
}

CSubWeapon* CSubWeapon::GetInstance()
{
	if (__instance == NULL)
		__instance = new CSubWeapon();

	return __instance;
}