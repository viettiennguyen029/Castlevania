#include "Dagger.h"
#include "Candle.h"

CDagger::CDagger() : CGameObject()
{
	vx = DAGGER_SPEED;
}

void CDagger::Render()
{
	animation_set->at(0)->Render(x, y, nx);
}

void CDagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (nx < 0) vx = -DAGGER_SPEED;
	else vx = DAGGER_SPEED;
	vy = 0;

	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx;
		y += min_ty * dy;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			// Collision between Dagger and Big Candle
			if (dynamic_cast<CCandle*>(e->obj))
			{
				DebugOut(L" Collision! Dagger and Candle\n");
				CCandle* candle = dynamic_cast<CCandle*>(e->obj);
				candle->SetState(CANDLE_DESTROYED);
				this->visible = false;
			}
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CDagger::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + DAGGER_BBOX_WIDTH;
	bottom = y + DAGGER_BBOX_HEIGHT;
}
