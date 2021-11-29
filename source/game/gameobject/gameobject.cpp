#include "GameObject.h"

GameObject::~GameObject()
{
	mName.clear();
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
	ObjectUpdate();
}

void GameObject::Draw()
{
	for (auto& component : componentList)
	{
		component->Draw();
	}
}
