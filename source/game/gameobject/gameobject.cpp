#include	"GameObject.h"

static int objectCnt = 0;

GameObject::GameObject(std::string mName, ObjectType mObjectType, bool mIsStopPose) :mName(mName), mObjectType(mObjectType), mIsStopPause(mIsStopPose)
{
	objectCnt++;
	mObjectID = objectCnt;
	mTransform.SetPtr(new Transform);
	SceneManager::GetInstance()->GetAddScene()->AddGameObject(this);
}

GameObject::~GameObject()
{
	SceneManager::GetInstance()->GetAddScene()->RemoveGameObject(this);

	mName.clear();
	for (auto &component : mComponentList)
	{
		if (component != nullptr)
		{
			delete(component);
		}
	}
	mComponentList.clear();
	mComponentList.shrink_to_fit();
}

void GameObject::Init()
{
	ObjectInit();
	for (auto& component : mComponentList)
	{
		component->Init();
	}
}

void GameObject::Update()
{
	ObjectUpdate();
	for (auto& component : mComponentList)
	{
		component->Update();
	}

	// 死んだコンポーネントを一時配列に追加
	std::vector<ComponentBase*> deadComponents;
	for (auto component : mComponentList)
	{
		if (!component->GetIsActive())
		{
			deadComponents.emplace_back(component);
		}
	}

	// 死んだアクターを消す（m_actorsから削除）
	for (auto component : deadComponents)
	{
		delete component;
	}

	mComponentList.shrink_to_fit();
}

void GameObject::Draw()
{
	ObjectDraw();
	for (auto& component : mComponentList)
	{
		component->Draw();
	}
}

void GameObject::ImguiDraw()
{
	if (ImGui::TreeNode(mName.c_str()))
	{
		std::string str;

		str = u8"ObjectID： " + std::to_string(mObjectID);
		ImGui::Text(str.c_str());

		if (ImGui::TreeNode("Transform"))
		{
			ImGui::Text("MtxPosition");
			ImGui::DragFloat("x", &mTransform->worldMtx._41, 0.5f);
			ImGui::DragFloat("y", &mTransform->worldMtx._42, 0.5f);
			ImGui::DragFloat("z", &mTransform->worldMtx._43, 0.5f);
			ImGui::Text("Position");
			ImGui::DragFloat("x", &mTransform->position.x, 0.5f);
			ImGui::DragFloat("y", &mTransform->position.y, 0.5f);
			ImGui::DragFloat("z", &mTransform->position.z, 0.5f);
			ImGui::Text("Angle");
			ImGui::DragFloat("x", &mTransform->angle.x, 0.5f);
			ImGui::DragFloat("y", &mTransform->angle.y, 0.5f);
			ImGui::DragFloat("z", &mTransform->angle.z, 0.5f);
			ImGui::Text("Sclale");
			ImGui::DragFloat("x", &mTransform->scale.x, 0.5f);
			ImGui::DragFloat("y", &mTransform->scale.y, 0.5f);
			ImGui::DragFloat("z", &mTransform->scale.z, 0.5f);
			ImGui::TreePop();
		}

		ObjectImguiDraw();

		// コンポーネント情報表示
		for (auto& component : mComponentList)
		{
			if (ImGui::TreeNode(component->GetName().c_str()))
			{
				component->ImguiDraw();
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}
}
