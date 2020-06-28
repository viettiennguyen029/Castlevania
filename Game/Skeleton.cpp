#include "Skeleton.h"

void CSkeleton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + SKELETON_BBOX_WIDTH;
	bottom = top + SKELETON_BBOX_HEIGHT;
}

void CSkeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

}

void CSkeleton::Render()
{
	animation_set->at(state)->Render(x, y, nx);
}

CSkeleton::CSkeleton(float x, float y)
{
	this->start_x = x;
	this->start_y = y;
	this->healthPoint = 2;
	SetState(SKELETON_STATE_DANCING);
}

void CSkeleton::SetState(int state)
{
	CGameObject::SetState(state);
}
