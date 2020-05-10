#include "Dagger.h"

CDagger::CDagger() : CGameObject()
{
	visible = false;
}

void CDagger::Render()
{
	animation_set->at(0)->Render(x, y, nx);
}

void CDagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (nx < 0) vx = -DAGGER_SPEED;
	else vx = DAGGER_SPEED;

	CGameObject::Update(dt);
	x += dx;
}

void CDagger::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + DAGGER_BBOX_WIDTH;
	bottom = y + DAGGER_BBOX_HEIGHT;
}
