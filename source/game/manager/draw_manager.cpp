#include		"draw_manager.h"
#include		"../component/draw_component_base.h"
#include		<algorithm>	// for sort


void DrawManager::AddDrawComponent(DrawComponentBase* _component)
{
	mpComponentList.emplace_back(_component);
}

void DrawManager::RemoveDrawComponent(DrawComponentBase* _component)
{
	if (mpComponentList.empty())
		return;
	auto it = std::find(mpComponentList.begin(), mpComponentList.end(), _component);
	if (it != mpComponentList.end())
	{
		mpComponentList.erase(it);
	}
	mpComponentList.shrink_to_fit();
}

void DrawManager::Draw()
{
	// ¸‡‚Åƒ\[ƒg‚·‚é
	std::sort(mpComponentList.begin(), mpComponentList.end(), [](const DrawComponentBase* lhoge, const DrawComponentBase* rhoge)
	{
		return lhoge->GetOrderInLayer() < rhoge->GetOrderInLayer();
	});

	for (auto &c : mpComponentList)
	{
		if (c->GetIsActiveWithOwner() && c->GetIsDraw())
			c->Draw();
	}
}
