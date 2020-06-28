#include "HolyWater.h"

CHolyWater* CHolyWater::__instance = NULL;

CHolyWater::CHolyWater()
{
	SetVisible(false);
	this->damage = 1;
}

CHolyWater* CHolyWater::GetInstance()
{
	if (__instance == NULL)__instance = new CHolyWater();
	return __instance;
}

void CHolyWater::ShowHitEffect()
{
	if (hitEffects.size() > 0)
	{
		if (startShow == 0)
		{
			startShow = GetTickCount();
		}

		else if (GetTickCount() - startShow > 100)
		{
			startShow = 0;
			hitEffects.clear();
		}

		// rendering hit effect based on the coordinate vector
		for (auto coord : hitEffects)
			hitEffect->Render(coord[0], coord[1], -1);
	}
}

void CHolyWater::Render()
{
	ShowHitEffect();
	animation_set->at(state)->Render(x, y, nx);
}

void CHolyWater::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CHolyWater::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

