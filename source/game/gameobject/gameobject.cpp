#include	"GameObject.h"

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
		if (ImGui::TreeNode("Transform"))
		{
			ImGui::Text("MtxPosition");
			ImGui::DragFloat("x", &mTransform.worldMtx._41, 0.5f);
			ImGui::DragFloat("y", &mTransform.worldMtx._42, 0.5f);
			ImGui::DragFloat("z", &mTransform.worldMtx._43, 0.5f);
			ImGui::Text("Position");
			ImGui::DragFloat("x", &mTransform.position.x, 0.5f);
			ImGui::DragFloat("y", &mTransform.position.y, 0.5f);
			ImGui::DragFloat("z", &mTransform.position.z, 0.5f);
			ImGui::TreePop();
		}
		for (auto& component : componentList)
		{
			component->ImguiDraw();
		}

		ImGui::TreePop();
	}
}
