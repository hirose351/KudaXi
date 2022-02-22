#include	"GameObject.h"

static int objectCnt = 0;

GameObject::GameObject(std::string mName, ObjectType mObjectType, bool mIsStopPose) :mName(mName), mObjectType(mObjectType), mIsStopPause(mIsStopPose), mIsActive(true)
{
	objectCnt++;
	mObjectID = objectCnt;
	mTransform.SetPtr(new Transform);
	mSceneKey = SceneManager::GetInstance()->GetCurrentSceneKey();
}

GameObject::~GameObject()
{
	mName.clear();
	for (auto &component : mpComponentList)
	{
		if (component != nullptr)
		{
			delete(component);
		}
	}
	mpComponentList.clear();
	mpComponentList.shrink_to_fit();
}

void GameObject::Init()
{
	ObjectInit();
	if (mpComponentList.empty())
		return;
	for (auto& component : mpComponentList)
	{
		component->Init();
	}
}

void GameObject::Update()
{
	if (mpParent != nullptr)
	{
		if (!mpParent->GetIsActive() && mObjectType != ObjectType::eStage&&mName != "NumButton")
			return;
		/*mTransform->CreateLocalMtx();
		DX11MtxMultiply(mTransform->worldMtx, mParent->GetTransform()->GetMtx(), mTransform->localMtx);*/
	}

	ObjectUpdate();
	for (auto& component : mpComponentList)
	{
		if (component->GetState() == ObjectState::eActive)
		{
			component->Update();
		}
	}

	// 死んだコンポーネントを一時配列に追加
	std::vector<ComponentBase*> deadComponents;
	for (auto component : mpComponentList)
	{
		if (component->GetState() == ObjectState::eDead)
		{
			deadComponents.emplace_back(component);
		}
	}

	// 死んだアクターを消す（m_actorsから削除）
	for (auto component : deadComponents)
	{
		delete component;
	}

	mpComponentList.shrink_to_fit();
}

void GameObject::ImguiDraw()
{
	if (ImGui::TreeNode(mName.c_str()))
	{
		std::string str;

		ImGui::Checkbox("isActive", &mIsActive);

		str = u8"ObjectID： " + std::to_string(mObjectID);
		ImGui::Text(str.c_str());

		if (ImGui::TreeNode("Transform"))
		{
			if (ImGui::TreeNode("MtxPosition"))
			{
				ImGui::DragFloat("x", &mTransform->worldMtx._41, 0.5f);
				ImGui::DragFloat("y", &mTransform->worldMtx._42, 0.5f);
				ImGui::DragFloat("z", &mTransform->worldMtx._43, 0.5f);
				ImGui::TreePop();
			}
			ImGui::DragFloat3("Position", &mTransform->position.x, 0.5f);
			ImGui::DragFloat3("Angle", &mTransform->angle.x, 0.5f);
			ImGui::DragFloat3("Rotation", &mTransform->rotation.x, 0.5f);
			ImGui::DragFloat3("Sclale", &mTransform->scale.x, 0.1f);
			ImGui::TreePop();
		}

		ObjectImguiDraw();

		// コンポーネント情報表示
		for (auto& component : mpComponentList)
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

void GameObject::ImguiCreateDraw()
{
	// コンポーネント情報表示
	for (auto& component : mpComponentList)
	{
		if (!component->GetIsCreate())
			continue;
		ImGui::Text(component->GetName().c_str());
		component->ImguiDraw();
		ImGui::Text("");
	}
}
