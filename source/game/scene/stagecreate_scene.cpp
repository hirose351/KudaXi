#include	"stagecreate_scene.h"
#include	"../gameobject/gameobject.h"
#include	"../gameobject/dice.h"
#include	"../gameobject/stage.h"
#include	"../gameobject/skydome.h"
#include	"../gameobject/access_dice_create_manager.h"
#include	"../manager/dice_manager.h"
#include	"../component/map_pos_component.h"
#include	"../component/map_move_component.h"
#include	"../component/collision_component.h"
#include	"../component/allcomponents.h"
#include	"../manager/stagedata_manager.h"
#include	"../manager/input_manager.h"
#include	"../../application.h"
#include	"../../system/dx11/DX11util.h"
#include	"../../system/util/XAudio2.h"

using namespace Dix;

enum {
	ePlayer,
	eStage,
	eDiceM
};

StageCreateScene::StageCreateScene()
{
}

void StageCreateScene::SceneAfter()
{
	StageDataManager::GetInstance().SetCurrentStage("create/init");
	mStageData = StageDataManager::GetInstance().GetCurrentStage();
	mSelectObjNum = 0;
	mStage->Reset();
	mStage->CameraUpdate();
	DiceManager::GetInstance()->Uninit();
}

void StageCreateScene::SceneInit()
{
	Dix::sp<GameObject> player;
	player.SetPtr(new GameObject("Player", ObjectType::ePlayer, false));
	player->AddComponent<Component::MapPos>();
	player->AddComponent<Component::MapMove>();
	player->AddComponent<Component::Model>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/player/player.fbx"));
	player->GetTransform()->SetPositionY(DICE_SCALE);
	player->GetTransform()->scale = Float3(5, 7, 5);
	player->AddComponent<Component::Collision>()->SetLocalScale(player->GetTransform()->scale);
	player->GetComponent<Component::Collision>()->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
	player->GetComponent<Component::Collision>()->SetOrderInLayer(30);
	AddGameObject(player);
	mViewObjList.emplace_back(player);

	Dix::sp<Stage> stage;
	stage.SetPtr(new Stage);
	mStage = stage;
	AddGameObject(stage);
	mViewObjList.emplace_back(stage);

	Dix::sp<Skydome> skydome;
	skydome.SetPtr(new Skydome);
	skydome->SetType(Skydome::Type::CLEATE);
	AddGameObject(skydome);

	Dix::sp<DiceCreateManagerAccess> dicemanager;
	dicemanager.SetPtr(new DiceCreateManagerAccess);
	AddGameObject(dicemanager);
	mViewObjList.emplace_back(dicemanager);
}

void StageCreateScene::SceneUpdate()
{
	if (mSelectObjNum == 0)
	{
		mViewObjList[0]->GetComponent<Component::Collision>()->SetIsDraw(true);
		mViewObjList[0]->GetComponent<Component::MapMove>()->SetState(ObjectState::eActive);
	}
	else
	{
		mViewObjList[0]->GetComponent<Component::Collision>()->SetIsDraw(false);
		mViewObjList[0]->GetComponent<Component::MapMove>()->SetState(ObjectState::ePaused);
	}
	DiceManager::GetInstance()->SetIsSelect(mSelectObjNum == eDiceM);

	// �߂�������ꂽ�Ƃ��̏���
	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eCancel2)))
		// �V�[����ύX
		SceneManager::GetInstance()->SetNextScene("Mode");
}

void StageCreateScene::ImguiDebug()
{
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 300), ImGuiCond_Once);
	ImGui::Begin(u8"GameObject");
	int cnt = 0;
	for (auto &obj : mViewObjList)
	{
		ImGui::RadioButton(obj->GetName().c_str(), &mSelectObjNum, cnt);
		cnt++;
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(900, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 350), ImGuiCond_Once);
	ImGui::Begin(u8"Inspector");
	ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);

	mViewObjList[mSelectObjNum]->ImguiCreateDraw();

	ImGui::End();

	RuleImGuiDraw();

	/// SaveLoad ///////////////////////////////////////////////////////////////////////////////////////////

	ImGui::SetNextWindowPos(ImVec2(900, 400), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 300), ImGuiCond_Once);
	ImGui::Begin(u8"SaveLoad");

	ImGui::InputText("StageName", mStageNameText, sizeof(mStageNameText));
	if (ImGui::Button("Save"))
	{
		StageDataSave();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		StageDataLoad();
	}
	ImGui::SameLine();
	if (ImGui::Button("Play"))
	{
		StageDataPlay();
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove"))
	{
		StageDataManager::GetInstance().RemoveStageData(mStageNameText);
	}
	if (ImGui::Button(u8"���݂̃X�e�[�W��Play"))
	{
		/// Todo:�������
	}
	ImGui::End();
}

void StageCreateScene::RuleImGuiDraw()
{
	ImGui::SetNextWindowPos(ImVec2(20, 400), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 150), ImGuiCond_Once);
	ImGui::Begin(u8"�Q�[�����[��");
	ImGui::Text(u8"�������̖�(����)");
	ImGui::SliderInt("TargetDiceType", &mStageData->mTargetDiceType, 1, 6);
	ImGui::Text(u8"STEP��");
	ImGui::SliderInt("step", &mStageData->mStep, 1, 30);
	ImGui::End();
}

void StageCreateScene::StageDataSave()
{
	StageData data = mStageData.At();

	data.mStageName = mStageNameText;
	data.mPlayerPos = mViewObjList[0]->GetComponent<Component::MapPos>()->GetMapPos();

	data.mMapSizeHeight;
	data.mMapSizeWidth;

	data.mDiceMtx.clear();
	data.mDiceMtx.shrink_to_fit();

	// �T�C�R���̐���Mtx�L��
	for (int z = 0; z < data.mMapSizeHeight; z++)
	{
		for (int x = 0; x < data.mMapSizeWidth; x++)
		{
			Dix::wp<Dice> dice = DiceManager::GetInstance()->GetCreateDice(INT2(x, z));

			if (dice == NULL)
			{
				data.mMap[z][x] = NODICE;
				continue;
			}
			// �z��ɓ����
			data.mDiceMtx.emplace_back();
			data.mDiceMtx[data.mDiceMtx.size() - 1] = dice->GetTransform()->GetMtx();
			data.mMap[z][x] = dice->GetObjectID();

		}
	}
	StageDataManager::GetInstance().SaveStage(data);
	mStageData->mStageName = "create/init";
}

void StageCreateScene::StageDataLoad()
{
	// �X�e�[�W�����[�h
	if (StageDataManager::GetInstance().LoadStage(mStageNameText))
	{
		// ���̃X�e�[�W�̃|�C���^���擾
		Dix::wp<StageData> p = StageDataManager::GetInstance().GetStageData(mStageNameText);

		mStageData.At() = p.At();
		mStageData->mStageName = "create/init";
		StageDataManager::GetInstance().SetCurrentStage(mStageData->mStageName);
		DiceManager::GetInstance()->DataCreate();
		mViewObjList[eDiceM]->Init();
		mStage->Init();
		mViewObjList[ePlayer]->GetComponent<Component::MapPos>()->SetMapPos(mStageData->mPlayerPos);
	}
}

void StageCreateScene::StageDataPlay()
{
	bool sts = StageDataManager::GetInstance().SetCurrentStage(mStageNameText);
	if (sts)
	{
		// �V�[���؂�ւ�
		StopSound(SOUND_LABEL_BGM_CREATE);
		PlaySound(SOUND_LABEL_BGM_GAME);
		SceneManager::GetInstance()->SetNextScene("GameMain");
	}
	else
	{
		MessageBox(nullptr, "�w�肳�ꂽ�X�e�[�W�f�[�^�͑��݂��܂���", "error", MB_OK);
		return;
	}
}

void StageCreateScene::Play()
{

}
