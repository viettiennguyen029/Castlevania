#include "Skeleton.h"
#include "Brick.h"
#include "Simon.h"

void CSkeleton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + SKELETON_BBOX_WIDTH;
	bottom = top + SKELETON_BBOX_HEIGHT;
}

void CSkeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{
	if (stopMoving == true)
	{
		stop = true;
		for (UINT i = 0; i < bones.size(); i++)
		{
			bones.at(i)->Update(dt, coObjects, stop);
		}
		return;
	}
	else
	{
		stop = false;
	}

	CGameObject::Update(dt);
	vy += 0.0011f * dt;

	if (start_untouchable != 0)
	{
		Untouchable();
	}

	
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	nx = (this->x > xS) ? -1 : 1;
	
	if (xS - x < 130 && state == SKELETON_STATE_IDLE)
	{
		SetState(SKELETON_STATE_ACTIVE);
	}

	if (rand() % 150 < 2)
	{
		LPGAMEOBJECT bone;
		bone = new CBone(x, y, nx);
		bones.push_back(bone);
	}

	for (UINT i = 0; i < bones.size(); i++)
	{
		bones.at(i)->Update(dt, coObjects, stop);
		if (bones.at(i)->IsInViewport() == false)
		{
			delete bones.at(i);
			bones.erase(bones.begin() + i);
		}
	}

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
		float min_tx, min_ty, nx, ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx;
		y += min_ty * dy;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT  e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{			
				float start1_x = 33;
				float end_x = 57;

				if (e->ny != 0)
				{

					int r = rand() % 10;
					if (r == 0)
					{
						canThrowBones = true;
					}

					vy = 0;
					y += ny * 0.4f; 
				}

				if (x >= end_x || x <= start1_x)
				{
					vx *= -1;
				}
			}
		}

		// clean up collision events
		for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}

}

void CSkeleton::Render()
{
	if (stop)
	{
		int currentFrame = animation_set->at(0)->GetCurrentFrame();
		animation_set->at(0)->SetCurrentFrame(currentFrame);
		animation_set->at(0)->RenderByFrame(currentFrame, nx, x, y);
	}
	else		
		animation_set->at(0)->Render(x, y, nx);

	// Rendering bones
	for (UINT i = 0; i < bones.size(); i++)
	{
		bones.at(i)->Render();
		//DebugOut(L"How many bones? :%d\n", (int)bones.size());
	}
}

CSkeleton::CSkeleton()
{
	this->healthPoint = 2;
	SetState(SKELETON_STATE_IDLE);
}

void CSkeleton::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SKELETON_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case SKELETON_STATE_ACTIVE:
		break;
	}
}

CSkeleton* CSkeleton::__instance = NULL;
CSkeleton* CSkeleton::GetInstance()
{
	if(__instance == NULL) __instance = new CSkeleton();
	return __instance;
}
