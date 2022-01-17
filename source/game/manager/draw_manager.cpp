#include		"draw_manager.h"
#include		"../component/draw_component_base.h"
#include		<algorithm>	// for sort


void DrawManager::AddDrawComponent(DrawComponentBase* _component)
{
	mComponentList.emplace_back(_component);
}

void DrawManager::RemoveDrawComponent(DrawComponentBase * _component)
{

	auto it = std::find(mComponentList.begin(), mComponentList.end(), _component);
	if (it != mComponentList.end())
	{
		mComponentList.erase(it);
	}
	mComponentList.shrink_to_fit();
}

void DrawManager::Draw()
{
	// ¸‡‚Åƒ\[ƒg‚·‚é
	std::sort(mComponentList.begin(), mComponentList.end(), [](const DrawComponentBase* lhoge, const DrawComponentBase* rhoge)
	{
		return lhoge->GetOrderInLayer() < rhoge->GetOrderInLayer();
	});

	for (auto &c : mComponentList)
	{
		if (c->GetIsActiveWithOwner())
			c->Draw();
	}
}
