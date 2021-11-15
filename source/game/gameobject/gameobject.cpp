#include "GameObject.h"
#include "../component/component_base.h"
#include "../component/allcomponents.h"

GameObject::~GameObject()
{
	name.clear();
	for (auto &component : componentList)
	{
		componentList.pop_back();
	}

	componentList.shrink_to_fit();
}

void GameObject::Update()
{
	for (auto& component : componentList)
	{
		component->Update();
	}
}

void GameObject::Draw()
{
	for (auto& component : componentList)
	{
		component->Draw();
	}
}

