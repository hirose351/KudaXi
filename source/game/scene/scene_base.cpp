#include	"scene_base.h"
#include	"../gameobject/gameobject.h"
#include	"../../system/imgui/util/myimgui.h"
#include	"../../system/dx11/DX11util.h"
#include	"../manager/draw_manager.h"

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
	mObjectList.clear();
	mObjectList.shrink_to_fit();
}

void SceneBase::AddGameObject(GameObject* _object)
{
	// アクターが更新中なら待ち群に追加
	if (mUpdatingActors || mInitingActors)
	{
		mPendingObjectList.emplace_back(_object);
	}
	else
	{
		mObjectList.emplace_back(_object);
	}
}

void SceneBase::RemoveGameObject(GameObject* _object)
{
	// 保留中のアクターかどうか
	auto iter = std::find(mPendingObjectList.begin(), mPendingObjectList.end(), _object);
	if (iter != mPendingObjectList.end())
	{
		// ベクトルの最後までワップしてポップオフします（コピーの消去は避けてください）
		std::iter_swap(iter, mPendingObjectList.end() - 1);
		mPendingObjectList.pop_back();
	}

	// 存在するアクターかどうか
	iter = std::find(mObjectList.begin(), mObjectList.end(), _object);
	if (iter != mObjectList.end())
	{
		// ベクトルの最後までワップしてポップオフします（コピーの消去は避けてください）
		std::iter_swap(iter, mObjectList.end() - 1);
		mObjectList.pop_back();
	}
}

bool SceneBase::Init()
{
	// すべてのオブジェクトを初期化
	mInitingActors = true;
	for (auto &obj : mObjectList)
	{
		obj->Init();
	}
	mInitingActors = false;

	// 待ちになっていたオブジェクトをリストに移動
	for (auto pending : mPendingObjectList)
	{
		pending->Init();
		mObjectList.emplace_back(pending);
	}
	mPendingObjectList.clear();

	return true;
}

void SceneBase::Update()
{
	// すべてのアクターを更新
	mUpdatingActors = true;
	for (auto &obj : mObjectList)
	{
		if (obj->GetObjectState() != ObjectState::eActive || !obj->GetIsActive())
			continue;
		if (obj->GetIsStopPause() && mIsPause)
			continue;
		obj->Update();
	}
	mUpdatingActors = false;

	// 待ちになっていたオブジェクトを本リストに移動
	for (auto pending : mPendingObjectList)
	{
		mObjectList.emplace_back(pending);
	}
	mPendingObjectList.clear();

	SceneUpdate();

	// 死んだオブジェクトを一時配列に追加
	std::vector<GameObject*> deadObjcts;
	for (auto obj : mObjectList)
	{
		if (obj->GetObjectState() == ObjectState::eDead)
		{
			deadObjcts.emplace_back(obj);
		}
	}

	// 死んだオブジェクトを消す（リストから削除）
	for (auto obj : deadObjcts)
	{
		delete obj;
	}

	mObjectList.shrink_to_fit();
}

void SceneBase::Render()
{
	mDrawManager.Draw();
	imguiDraw(std::bind(&SceneBase::ImguiDebug, std::ref(*this)));
}

void SceneBase::ImguiDebug()
{
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 300), ImGuiCond_Once);
	ImGui::Begin(u8"GameObject");
	for (auto &obj : mObjectList)
	{
		obj->ImguiDraw();
	}
	ImGui::End();

	static DirectX::XMFLOAT4 lightPos = { -50, -87, 66, 0 };

	ImGui::SetNextWindowPos(ImVec2(20, 300), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 180), ImGuiCond_Once);
	ImGui::Begin(u8"Scene");
	{
		ImGui::Text(u8"現在のFPS : %.1f FPS", ImGui::GetIO().Framerate);
		if (ImGui::TreeNode("Light"))
		{
			ImGui::DragFloat3("Light", &lightPos.x, 0.5f);
			DX11LightInit(lightPos);	// 平行光源をセット
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Camera"))
		{
			ImGui::DragFloat3("Lookat", &mCameraLookat.x, 0.5f);
			CCamera::GetInstance()->SetLookat(mCameraLookat);
			CCamera::GetInstance()->CreateCameraMatrix();
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

bool SceneBase::Dispose()
{
	for (auto &obj : mObjectList)
	{
		if (obj != nullptr)
		{
			delete(obj);
		}
	}
	return true;
}

void SceneBase::AddDrawComponent(DrawComponentBase* _c)
{
	mDrawManager.AddDrawComponent(_c);
}

void SceneBase::RemoveDrawComponent(DrawComponentBase * _c)
{
	mDrawManager.RemoveDrawComponent(_c);
}
