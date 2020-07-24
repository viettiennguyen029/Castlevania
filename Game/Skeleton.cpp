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
	

	if (start_untouchable != 0)
	{
		Untouchable();
	}

	
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	this->nx = (this->x > xS) ? -1 : 1;

	vector<LPGAMEOBJECT> listBrick;

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CBrick*>(coObjects->at(i)))
		{
			listBrick.push_back(coObjects->at(i));
		}
	}

	if ((xS - x < 105) && (yS - y>=49) &&state == SKELETON_STATE_IDLE)
	{
		SetState(SKELETON_STATE_ACTIVE);
		DebugOut(L"Active Skeleton \n");
	}

	if (rand() % 150 < 2 && jumping == false)
	{
		LPGAMEOBJECT bone;
		bone = new CBone(x, y, nx);
		bones.push_back(bone);
	}

	for (UINT i = 0; i < bones.size(); i++)
	{
		bones.at(i)->Update(dt, coObjects, stop);
		if (bones.at(i)->IsInViewport() == false)
			bones.erase(bones.begin() + i);
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(&listBrick, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if(onGround)
			jumping = true;
	}

	else
	{
		float min_tx, min_ty, nx, ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx+ nx * 0.2f;
		y += min_ty * dy+ ny*0.2f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT  e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{					
				if (e->ny !=0)
				{
					if (e->ny == -1)
					{
						vy = 0;						
						onGround = true;
					}
					else y += dy;						
				}

				if (e->nx != 0 ) 
					vx = -vx;
			}
		}		
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
	listBrick.clear();


	CGameObject::Update(dt);
	vy += 0.001f * dt;

	if (jumping)
	{
		vy = -0.24f;
		onGround = jumping = false;
	}


	if (abs(xS - x) > 98 && state != SKELETON_STATE_IDLE)
	{
		//DebugOut(L"Skeleton froce to Simon\n");
		vx = 0.08 * nx;
	}

	if(abs(x - xS) < 22 && abs(y - yS) < 47)
	{
		if (coEvents.size() > 0)
		{
			vx = -0.06 * nx;
			//DebugOut(L"Skeleton try to avoid to Simon\n");
		}			
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

 vector <LPGAMEOBJECT> CSkeleton::GetBones()
{
	return this->bones;
}
