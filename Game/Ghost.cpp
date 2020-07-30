#include "Ghost.h"
#include "Simon.h"

void CGhost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GHOST_BBOX_WIDTH;
	bottom = y + GHOST_BBOX_HEIGHT;
}

void CGhost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool stopMoving)
{
	if (stopMoving)
	{
		stop = true;
		return;
	}
	else
	{
		stop = false;
	}

	CGameObject::Update(dt);
	// Update the position
	x += dx;
	y += dy;

	DetectPlayer();

	if (vy==0)
	{
		SetState(GHOST_STATE_FLYING);
	}

	if (CSimon::GetInstance()->GetUntouchable() == 1)
	{
		SetState(GHOST_STATE_ATTACK_PLAYER);
	}


	if (y + GHOST_BBOX_HEIGHT <= start_y)
	{
		vy = GHOST_FLYING_SPEED_VY;
		flyingup = true;
	}
	if (y >= start_y && flyingup)
	{
		vy = 0;
		flyingup = false;
	}

	if (start_untouchable != 0)
	{
		Untouchable();
	}

	if (!this->IsInViewport())
	{
		SetVisible(false);
	}
}

void CGhost::Render()
{
	if (stop)
	{
		int currentFrame = animation_set->at(0)->GetCurrentFrame();
		animation_set->at(0)->SetCurrentFrame(currentFrame);
		animation_set->at(0)->RenderByFrame(currentFrame, nx, x, y);
	}
	else
		animation_set->at(0)->Render(x, y, nx);
}

void CGhost::DetectPlayer()
{
	float xS, yS;
	CSimon::GetInstance()->GetPosition(xS, yS);

	if (xS > x + GHOST_BBOX_WIDTH)
	{
		SetOrientation(1);
	}
	else if (xS + SIMON_BBOX_WIDTH < x)
	{
		SetOrientation(-1);
	}
}

CGhost::CGhost()
{
	this->healthPoint = 3;
	this->nx = -1;
	SetState(GHOST_STATE_FLYING);
	SetVisible(false);
}

void CGhost::SetState(int state)
{
	
	switch (state)
	{
	case GHOST_STATE_FLYING:
	{
		vy = 0;
		vx = (nx > 0) ? GHOST_FLYING_SPEED_VX : -GHOST_FLYING_SPEED_VX;
		this->GetPosition(x, start_y);
		break;
	}

	case GHOST_STATE_ATTACK_PLAYER:
	{
		vy = -GHOST_FLYING_SPEED_VY;
		vx = (nx > 0) ? GHOST_FLYING_SPEED_VX : -GHOST_FLYING_SPEED_VX;
		break;
	}
	default:
		break;
	}

}


void CSpawnGhost::Add(int obj_type, LPGAMEOBJECT ghost)
{
	ghost->SetVisible(false);
	this->ghosts[obj_type].push_back(ghost);
}

void CSpawnGhost::SetVisible()
{
	LPGAMEOBJECT ghost = Get(1);
	if (ghost == NULL)
	{
		return;
	}
	else
	{
		ghost->SetVisible(true);
	}
}

LPGAMEOBJECT CSpawnGhost::Get(int obj_type)
{
	if (ghosts[obj_type].empty())
		DebugOut(L"[ERROR] Ghost not found \n");
	else
	{
		for (auto i = ghosts[obj_type].begin(); i != ghosts[obj_type].end(); ++i)
		{
			return (*i);
			break;
		}
	}
}

void CSpawnGhost::Clear()
{
	ghosts.clear();
}

CSpawnGhost* CSpawnGhost::__instance = NULL;
CSpawnGhost* CSpawnGhost::GetInstance()
{
	if (__instance == NULL)
		__instance = new CSpawnGhost();

	return __instance;
}
