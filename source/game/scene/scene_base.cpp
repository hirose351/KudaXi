#include	"scene_base.h"
#include	"../gameobject/gameobject.h"
#include	"../../system/imgui/util/myimgui.h"
#include	"../../system/dx11/DX11util.h"
#include	"../manager/draw_manager.h"
#include	"../gameobject/fade_screen.h"

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
	//Dispose();
	mpObjectList.clear();
}

void SceneBase::AddGameObject(Dix::sp<GameObject> _object)
{
	// アクターが更新中なら待ち群に追加
	if (mUpdatingObjects || mInitingObjects)
	{
		//Dix::sp<GameObject> spObject(_object);
		mpPendingObjectList.emplace_back(_object);
	}
	else
	{
		//Dix::sp<GameObject> spObject(_object);
		mpObjectList.emplace_back(_object);
	}
}

void SceneBase::RemoveGameObject(GameObject* _object)
{
	//// 保留中のアクターかどうか
	//auto iter = std::find(mpPendingObjectList.begin(), mpPendingObjectList.end(), _object);
	//if (iter != mpPendingObjectList.end())
	//{
	//	//// ベクトルの最後までワップしてポップオフします（コピーの消去は避けてください）
	//	//std::iter_swap(iter, mpPendingObjectList.end() - 1);
	//	//mpPendingObjectList.pop_back();
	//	mpPendingObjectList.erase(iter);
	//}

	//// 存在するアクターかどうか
	//iter = std::find(mpObjectList.begin(), mpObjectList.end(), _object);
	//if (iter != mpObjectList.end())
	//{
	//	//// ベクトルの最後までワップしてポップオフします（コピーの消去は避けてください）
	//	//std::iter_swap(iter, mpObjectList.end() - 1);
	//	//mpObjectList.pop_back();
	//	mpObjectList.erase(iter);
	//}
}

bool SceneBase::Init()
{
	mIsPause = false;
	if (fade == nullptr)
		fade = new FadeScreen;

	// すべてのオブジェクトを初期化
	mInitingObjects = true;
	for (auto &obj : mpObjectList)
	{
		obj->Init();
	}
	mInitingObjects = false;

	// 待ちになっていたオブジェクトをリストに移動
	for (auto pending : mpPendingObjectList)
	{
		pending->Init();
		mpObjectList.emplace_back(pending);
	}
	mpPendingObjectList.clear();

	return true;
}

void SceneBase::Update()
{
	if (!fade->GetIsCompleted())
	{
		fade->Update();
		return;
	}

	// すべてのアクターを更新
	mUpdatingObjects = true;
	for (auto &obj : mpObjectList)
	{
		if (obj->GetObjectState() != ObjectState::eActive || !obj->GetIsActive())
			continue;
		if (obj->GetIsStopPause() && mIsPause)
			continue;
		obj->Update();
	}
	mUpdatingObjects = false;

	// 待ちになっていたオブジェクトを本リストに移動
	for (auto pending : mpPendingObjectList)
	{
		mpObjectList.emplace_back(pending);
	}
	mpPendingObjectList.clear();

	SceneUpdate();

	// 死んだオブジェクトを一時配列に追加
	std::list<Dix::sp<GameObject>> deadObjcts;
	for (auto obj : mpObjectList)
	{
		if (obj->GetObjectState() == ObjectState::eDead)
		{
			deadObjcts.emplace_back(obj);
		}
	}

	// 死んだオブジェクトを消す（リストから削除）
	for (auto obj : deadObjcts)
	{
		obj.Clear();
	}

	//mpObjectList.shrink_to_fit();
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
	for (auto &obj : mpObjectList)
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
			Camera::GetInstance()->SetLookat(mCameraLookat);
			Camera::GetInstance()->CreateCameraMatrix();
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

bool SceneBase::Dispose()
{
	//for (auto &obj : mpObjectList)
	//{
	//	if (obj != nullptr)
	//	{
	//		delete(obj);
	//	}
	//}
	return true;
}

void SceneBase::DrawFadeIn()
{
	fade->SetFadeType(FadeType::eIn);
}

void SceneBase::DrawFadeOut()
{
	fade->SetFadeType(FadeType::eOut);
}

void SceneBase::AddDrawComponent(DrawComponentBase* _c)
{
	mDrawManager.AddDrawComponent(_c);
}

void SceneBase::RemoveDrawComponent(DrawComponentBase * _c)
{
	mDrawManager.RemoveDrawComponent(_c);
}
