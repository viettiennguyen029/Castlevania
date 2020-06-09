#include "Items.h"

CItems* CItems::__instance = NULL;

void CItems::AddItem(int itemType, LPGAMEOBJECT item)
{
	items[itemType].push_back(item);
}

void CItems::CheckAndDrop(LPGAMEOBJECT Object)
{
	int itemType = Object->GetItemId();
	float x, y;
	Object->GetPosition(x, y);
	Drop(itemType, x,y);
}

void CItems::Drop(int itemType, float x, float y)
{
	for (auto it = items[itemType].begin(); it != items[itemType].end(); ++it)
	{
		if ((*it)->isVisible() == false)
		{
			(*it)->SetPosition(x, y);
			(*it)->SetVisible(true);
			break;
		}
	}
}

void CItems::Clear()
{
	items.clear();
}



CItems* CItems::GetInstance()
{
	if (__instance == NULL)
		__instance = new CItems();

	return __instance;
}
