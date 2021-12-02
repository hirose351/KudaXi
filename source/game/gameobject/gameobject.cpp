#include "GameObject.h"

GameObject::~GameObject()
{
	mName.clear();
	for (auto &component : componentList)
	{
		componentList.pop_back();
	}
	componentList.clear();
	componentList.shrink_to_fit();
}

void GameObject::Init()
{
	ObjectInit();
	for (auto& component : componentList)
	{
		component->Init();
	}
}

void GameObject::Update()
{
	ObjectUpdate();
	for (auto& component : componentList)
	{
		component->Update();
	}
}

void GameObject::Draw()
{
	ObjectDraw();
	for (auto& component : componentList)
	{
		component->Draw();
	}
}

void GameObject::ImguiDraw()
{
	if (ImGui::TreeNode(mName.c_str()))
	{

	}
}
