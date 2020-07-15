#include "Bones.h"

CBones::CBones()
{

}

void CBones::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BONES_BBOX_WIDTH;
	bottom = y + BONES_BBOX_HEIGHT;
}

void CBones::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	//vy += 0.0005f * dt; //gravity
}

void CBones::Render()
{
	animation_set->at(0)->Render(x, y, nx);
}
