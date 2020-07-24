#include "Bone.h"

CBone::CBone(float x,float y,int nx)
{
	this->start_x = x;
	SetPosition(x, y);
	// CAnimationSets::GetInstance()->Get(69);
	LPANIMATION_SET bone_ani_set = CAnimationSets::GetInstance()->Get(69);
	SetAnimationSet(bone_ani_set);
	SetOrientation(nx);

}

void CBone::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BONE_BBOX_WIDTH;
	bottom = y + BONE_BBOX_HEIGHT;
}

void CBone::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{

	if (stopMoving == true)
	{
		stop = true;
		return;
	}
	else
	{
		stop = false;
	}

	CGameObject::Update(dt);
 	vy += 0.0005f * dt; //gravity

	if (abs(x - start_x) <= 4) 
	{
		vx = (nx > 0) ? BONE_FLY_SPEED_VX * dt : -BONE_FLY_SPEED_VX * dt;
		//vy = -0.22f;

		int r = rand() % 2;
		if (r == 0)
		{
			vy = -0.08f;
		}
		else
		{
			vy = -0.12f;
		}
	}
	else
	{
		vx = (nx > 0) ? BONE_FLY_SPEED_VX * dt : -BONE_FLY_SPEED_VX * dt;
		if (abs(x - start_x) >= 60)
			vy = 0.08f;
	}

	if (!IsInViewport())
	{
		SetVisible(false);
	}

	x += dx;
	y += dy;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx;
		y += min_ty * dy;

	}

	// Clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBone::Render()
{
	if (stop)
	{
		int currentFrame = animation_set->at(state)->GetCurrentFrame();
		animation_set->at(state)->SetCurrentFrame(currentFrame);
		animation_set->at(state)->RenderByFrame(currentFrame, nx, x, y);
	}
	else
		animation_set->at(state)->Render(x, y, nx);
}