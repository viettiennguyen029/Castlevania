#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Sprites.h"

#define HIT_EFFECT_ANIMATION	299
/*
Sprite animation
*/
class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;
	D3DXVECTOR2 position;
public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
	D3DXVECTOR2 GetPosition() { return this->position; }
};

typedef CAnimationFrame *LPANIMATION_FRAME;

/*
Manage all frames of an animation
*/
class CAnimation
{
	DWORD lastFrameTime;
	DWORD aniStartTime;   // mốc thời gian kể từ lúc bắt đầu render một animation

	int currentFrame;
	int defaultTime;
	

	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);
	int GetCurrentFrame() { return currentFrame; }
	void SetCurrentFrame(int frame) { this->currentFrame = frame; }
	void Render(float x, float y, int nx =1, int alpha = 255);
	void RenderByFrame(int fameID, int nx, float x, float y, int alpha = 255); // Specific function for rendering whip
	void Reset() { currentFrame = -1; }
	void SetAniStartTime(DWORD t) { aniStartTime = t; }
	D3DXVECTOR2 GetFramePosition() { return frames[currentFrame]->GetPosition(); }
	bool IsOver(DWORD dt) { return GetTickCount() - aniStartTime >= dt; }
};

typedef CAnimation *LPANIMATION;

class CAnimations
{
	static CAnimations * __instance;

	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	void Clear();
	void Release(int id);
	static CAnimations * GetInstance();
};

typedef vector<LPANIMATION> CAnimationSet;

typedef CAnimationSet* LPANIMATION_SET;

/*
	Manage animation set database
*/
class CAnimationSets
{
	static CAnimationSets * __instance;

	unordered_map<int, LPANIMATION_SET> animation_sets;

public:
	CAnimationSets();
	void Add(int id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(unsigned int id);
	void Release(unsigned int id);

	static CAnimationSets * GetInstance();
};