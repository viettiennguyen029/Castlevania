#include "PointEffects.h"

PointEffect::PointEffect()
{
	SetVisible(true);
}

void PointEffect::SetState(int state)
{
	this->state = state;
}

void PointEffect::Render()
{
	animation_set->at(state)->Render(x, y, -1);
	//RenderBoundingBox();
}

void PointEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	CGameObject::Update(dt);
	if (visible)
	{
		if (start_visible < POINT_EFFECT_LIFE_SPAN)
		{
			start_visible += dt;
		}
		else
		{
			SetVisible(false);
			start_visible = 0;
		}
	}
}

void PointEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 14;
	b = y + 8;
}

PointEffects* PointEffects::__instance = NULL;
PointEffects* PointEffects::GetInstance()
{
	if (__instance == NULL)
		__instance = new PointEffects();
	return __instance;
}

void PointEffects::Add(PointEffect* point)
{
	point->SetVisible(false);
	points.push_back(point);
}

void PointEffects::ShowPoint(int state,float x, float y)
{
	PointEffect* point = NULL;
	UINT i = 0;
	for (; i < points.size(); i++)
	{
		if (points[i]->isVisible() == false)
		{
			point = points[i];
			i++;
			break;
		}
	}
	if (point != NULL)
	{
		point->SetPosition(x, y);
		point->SetState(state);
		point->SetVisible(true);
	}
}

void PointEffects::Clear()
{
	points.clear();
}
