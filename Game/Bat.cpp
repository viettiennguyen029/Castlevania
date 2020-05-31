#include "Bat.h"
#include "Simon.h"
CBat::CBat() : CGameObject()
{
	flyingDownTime = 0;
	vx = vy = 0;
}

void CBat::Render()
{
	int ani = -1;
	if (vx == 0)
		ani = 0;
	else
		ani = 1;
	animation_set->at(ani)->Render(x, y, nx);
}

void CBat::SetState(int state)
{
	CGameObject::SetState(state);	
	
}

void CBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	CGameObject::Update(dt);
	
	/*if (CSimon::GetInstance()->ActiveEnemies() == true)
	{
		vy = 0.06f;
		vx = 0.06f;

		if (y > 40)
		{
			vx = 0.08f;
			vy = 0;
		}
		
	}
	else
	{
		vx = vy = 0;
	}

	x += dx;
	y += dy;*/


}

void CBat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BAT_BBOX_WIDTH;
	bottom = y + BAT_BBOX_HEIGHT;
}
