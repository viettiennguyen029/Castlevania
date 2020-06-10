#include "HitEffect.h"
#include "GameObject.h"
class FlameEffect
{
    vector<HitEffect*> flames;
    static FlameEffect* __instance;

public:
    void Add(HitEffect* flame);
    void  Show(LPGAMEOBJECT obj);
    FlameEffect() { ; }
    static FlameEffect* GetInstance();
};