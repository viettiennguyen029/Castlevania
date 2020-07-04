#include <d3dx9.h>
#include <algorithm>

#include "Utils.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"
#include "Items.h"
#define  UNTOUCHABLE_TIME	200
CGameObject::CGameObject()
{
	state = 0;
	x = y = 0;
	vx = vy = 0;
	nx = 1;	
	visible = true;
	healthPoint = 0;
	damage = 0;
	score = 0;
}

void CGameObject::Update(DWORD dt,vector<LPGAMEOBJECT>* coObjects)
{
	this->dt = dt;
	dx = vx*dt;
	dy = vy*dt;
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx*dt;
	float sdy = svy*dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent * e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects 
	
	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT> *coObjects, 
	vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT> &coEvents,
	vector<LPCOLLISIONEVENT> &coEventsResult,
	float &min_tx, float &min_ty, 
	float &nx, float &ny, float &rdx, float &rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty  && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
	}

	if (min_ix>=0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy>=0) coEventsResult.push_back(coEvents[min_iy]);
}

void CGameObject::Untouchable()
{
	if (GetTickCount() - start_untouchable > UNTOUCHABLE_TIME)
		start_untouchable = 0;
	else 
		vx = vy = 0;
}

void CGameObject::TakeDamage(int damage)
{
	if (start_untouchable == 0)
	{
		if (healthPoint > 0)
			healthPoint -= damage;

		if (healthPoint <= 0)
		{
			this->Die();
		}

		else
			start_untouchable = GetTickCount();
	}
}

void CGameObject::Die()
{	
	this->SetVisible(false);
	CItems::GetInstance()->CheckAndDrop(this);
}

bool CGameObject::AABB(float left_a, float top_a, float right_a, float bottom_a, float left_b, float top_b, float right_b, float bottom_b)
{
	
	return left_a < right_b&& right_a > left_b && top_a < bottom_b&& bottom_a > top_b;
}

void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(l, t, 1, bbox, 0, 0, rect.right, rect.bottom, 190);
}

bool CGameObject::IsOverlapping(LPGAMEOBJECT obj)
{
	float left, top, right, bottom;
	float leftObj, topObj, rightObj, bottomObj;

	obj->GetBoundingBox(leftObj, topObj, rightObj, bottomObj);
	GetBoundingBox(left, top, right, bottom);

	if (left < rightObj && right > leftObj &&
		top < bottomObj && bottom > topObj)
		return true;

	return false;
}

void CGameObject::ReDirect(float positionX)
{
	if (positionX - x == 0) { /* Do nothing. */ }
	else if (positionX - x < 0) nx = -1;
	else	nx = 1;
}

bool CGameObject::IsInViewport()
{
	// The viewport bounding box
	float vpLeft, vpTop, vpRight, vpBottom;
	CGame::GetInstance()->GetCameraBoundingBox(vpLeft, vpTop, vpRight, vpBottom);
	
	// The object bounding box
	float left, top, right, bottom;
	this->GetBoundingBox(left, top, right, bottom);

	if (vpLeft > right || vpTop > bottom|| vpRight < left || vpBottom < top)
		return false;

	return true;
}


CGameObject::~CGameObject()
{

}

void CGameObject::ResetAnimation()
{
	for (auto iter : animations)
		iter->Reset();
}