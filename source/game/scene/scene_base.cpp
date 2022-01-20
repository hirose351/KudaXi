#include	"scene_base.h"
#include	"../gameobject/gameobject.h"
#include	"../../system/imgui/util/myimgui.h"
#include	"../../system/dx11/DX11util.h"
#include	"../manager/draw_manager.h"
#include	"../manager/dice_manager.h"
#include	"../gameobject/fade_screen.h"
#include	"../../system/dx11/CDirectInput.h"

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
		mpPendingObjectList.emplace_back(_object);
	}
	else
	{
		mpObjectList.emplace_back(_object);
	}
}

bool SceneBase::Init()
{
	mIsPause = false;
	if (fade == nullptr)
		fade = new FadeScreen;

	DiceManager::GetInstance()->Uninit();
	// 死んだオブジェクトを消す（リストから削除）
	for (auto obj = mpObjectList.begin(); obj != mpObjectList.end();)
	{
		if ((*obj)->GetObjectState() == ObjectState::eDead)
		{
			(*obj).Clear();
			// 削除された要素の次を指すイテレータが返される。
			obj = mpObjectList.erase(obj);
		}
		else
		{
			obj++;
		}
	}

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
		for (auto obj : mpObjectList)
		{
			if (obj->GetObjectType() == ObjectType::eStage)
			{
				obj->Update();
				break;
			}
		}
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

	// 死んだオブジェクトを消す（リストから削除）
	for (auto obj = mpObjectList.begin(); obj != mpObjectList.end();)
	{
		if ((*obj)->GetObjectState() == ObjectState::eDead)
		{
			(*obj).Clear();
			// 削除された要素の次を指すイテレータが返される。
			obj = mpObjectList.erase(obj);
		}
		else
		{
			obj++;
		}
	}
}

void SceneBase::Render()
{
	mDrawManager.Draw();

	static bool isimguidraw = true;
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_TAB))
		isimguidraw = !isimguidraw;
	if (isimguidraw)
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
			ImGui::DragFloat3("Eye", &mCameraEye.x, 0.5f);
			Camera::GetInstance()->SetEye(mCameraEye);
			ImGui::DragFloat3("Lookat", &mCameraLookat.x, 0.5f);
			Camera::GetInstance()->SetLookat(mCameraLookat);
			ImGui::DragFloat3("Up", &mCameraUp.x, 0.5f);
			Camera::GetInstance()->SetUp(mCameraUp);
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
