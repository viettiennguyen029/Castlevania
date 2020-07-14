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

	case (int)SubWeapon::AXE:
		UseAxe();
		break;

	default:
		break;
	}
}

void CSubWeapon::UseDagger()
{
	LPGAMEOBJECT dagger = GetWeapon((int)SubWeapon::DAGGER);
	if (dagger == NULL) return;
	dagger->SetVisible(true);
	dagger->SetOrientation(CSimon::GetInstance()->GetOrientation());
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);
	int nx = CSimon::GetInstance()->GetOrientation();
	dagger->SetPosition(xS + (10 * nx), yS + 5);
}

void CSubWeapon::UseBoomerang()
{
	LPGAMEOBJECT boomerang = GetWeapon((int)SubWeapon::BOOMERANG);
	if (boomerang == NULL) return;
	boomerang->SetVisible(true);
	boomerang->SetOrientation(CSimon::GetInstance()->GetOrientation());
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);
	int nx = CSimon::GetInstance()->GetOrientation();
	boomerang->SetPosition(xS+(10*nx), yS+5);
}

void CSubWeapon::UseHolyWater()
{
	LPGAMEOBJECT holywater = GetWeapon((int)SubWeapon::HOLYWATER);
	if (holywater == NULL) return;
	holywater->SetVisible(true);
	holywater->SetOrientation(CSimon::GetInstance()->GetOrientation());
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);
	int nx = CSimon::GetInstance()->GetOrientation();
	holywater->SetPosition(xS + (11 * nx), yS + 6);
}

void CSubWeapon::UseAxe()
{
	LPGAMEOBJECT axe = GetWeapon((int)SubWeapon::AXE);
	if (axe == NULL) return;
	axe->SetVisible(true);
	axe->nx = CSimon::GetInstance()->GetOrientation();
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);
	int nx = CSimon::GetInstance()->GetOrientation();
	axe->SetPosition(xS + (4 * nx), yS );
}

LPGAMEOBJECT CSubWeapon::GetWeapon(int weapon)
{
	if (CSubWeapon::GetInstance()->subweapons[weapon].empty())
		DebugOut(L"\n[ERROR] No weapons");

	else
	{
		for (auto i = CSubWeapon::GetInstance()->subweapons[weapon].begin(); i != CSubWeapon::GetInstance()->subweapons[weapon].end(); ++i)
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