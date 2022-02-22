#include	"scene_base.h"
#include	"../gameobject/gameobject.h"
#include	"../gameobject/fade_screen.h"
#include	"../../system/imgui/util/myimgui.h"
#include	"../../system/dx11/dx11_util.h"
#include	"../../system/dx11/direct_input.h"
#include	"../manager/draw_manager.h"
#include	"../manager/dice_manager.h"

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
	mpObjectList.clear();
}

void SceneBase::AddGameObject(Dix::sp<GameObject> _object)
{
	// �A�N�^�[���X�V���Ȃ�҂��Q�ɒǉ�
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
	if (mpfade == nullptr)
		mpfade = new FadeScreen;

	DiceManager::GetInstance()->Uninit();
	// ���񂾃I�u�W�F�N�g�������i���X�g����폜�j
	for (auto obj = mpObjectList.begin(); obj != mpObjectList.end();)
	{
		if ((*obj)->GetObjectState() == ObjectState::eDead)
		{
			(*obj).Clear();
			// �폜���ꂽ�v�f�̎����w���C�e���[�^���Ԃ����B
			obj = mpObjectList.erase(obj);
		}
		else
		{
			obj++;
		}
	}

	// ���ׂẴI�u�W�F�N�g��������
	mInitingObjects = true;
	for (auto &obj : mpObjectList)
	{
		obj->Init();
	}
	mInitingObjects = false;

	// �҂��ɂȂ��Ă����I�u�W�F�N�g�����X�g�Ɉړ�
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
	if (!mpfade->GetIsCompleted())
	{
		mpfade->Update();
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

	// ���ׂẴA�N�^�[���X�V
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

	// �҂��ɂȂ��Ă����I�u�W�F�N�g��{���X�g�Ɉړ�
	for (auto pending : mpPendingObjectList)
	{
		mpObjectList.emplace_back(pending);
	}
	mpPendingObjectList.clear();

	SceneUpdate();

	// ���񂾃I�u�W�F�N�g�������i���X�g����폜�j
	for (auto obj = mpObjectList.begin(); obj != mpObjectList.end();)
	{
		if ((*obj)->GetObjectState() == ObjectState::eDead)
		{
			(*obj).Clear();
			// �폜���ꂽ�v�f�̎����w���C�e���[�^���Ԃ����B
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

	static bool isImguiDraw = false;
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_TAB))
		isImguiDraw = !isImguiDraw;
	if (isImguiDraw || mpSceneManager->GetCurrentSceneKey() == "Create")
		ImguiDraw(std::bind(&SceneBase::ImguiDebug, std::ref(*this)));
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

	ImGui::SetNextWindowPos(ImVec2(20, 320), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 180), ImGuiCond_Once);
	ImGui::Begin(u8"Scene");
	{
		ImGui::Text(u8"���݂�FPS : %.1f FPS", ImGui::GetIO().Framerate);
		if (ImGui::TreeNode("Light"))
		{
			ImGui::DragFloat3("Light", &lightPos.x, 0.5f);
			DX11LightInit(lightPos);	// ���s�������Z�b�g
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
	mpfade->SetFadeType(FadeType::eIn);
}

void SceneBase::DrawFadeOut()
{
	mpfade->SetFadeType(FadeType::eOut);
}

void SceneBase::AddDrawComponent(DrawComponentBase* _c)
{
	mDrawManager.AddDrawComponent(_c);
}

void SceneBase::RemoveDrawComponent(DrawComponentBase * _c)
{
	mDrawManager.RemoveDrawComponent(_c);
}
