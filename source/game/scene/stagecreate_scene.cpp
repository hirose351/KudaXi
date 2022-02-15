
#define _CRT_SECURE_NO_WARNINGS

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
#include	<stdio.h>
#include	<string.h>

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
	if (SceneManager::GetInstance()->GetBeforeSceneKey() == "Mode")
	{
		StageDataManager::GetInstance().SetCurrentStage("create/init");
		mStageData = StageDataManager::GetInstance().GetCurrentStage();
		mStageData->mStageName = "create/init";
		mSelectObjNum = 0;
		mStage->Reset();
		DiceManager::GetInstance()->Uninit();

		StopSound(SOUND_LABEL_BGM_TITLE);
	}
	else
	{
		DiceManager::GetInstance()->DataCreate();
		mViewObjList[eDiceM]->Init();
		mStage->Init();
		mViewObjList[ePlayer]->GetComponent<Component::MapPos>()->SetMapPos(mStageData->mPlayerPos);
		char stageNameText[128] = "create/play";
		if (mIsStagePlay)
		{
			strcpy(mStageNameText, mBeforeStageName.c_str());
		}

		StopSound(SOUND_LABEL_BGM_GAME);
	}

	mStage->CameraUpdate();
	PlaySound(SOUND_LABEL_BGM_CREATE);
	mIsStagePlay = false;
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

	// 戻るを押されたときの処理
	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eCancel2)))
		// シーンを変更
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
	if (ImGui::Button(u8"保存"))
	{
		StageDataSave();
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"上書き保存"))
	{
		StageDataManager::GetInstance().RemoveStageData(mStageNameText);
		StageDataSave();
	}
	if (ImGui::Button(u8"読み込み"))
	{
		StageDataLoad();
	}
	if (ImGui::Button("Play"))
	{
		StageDataPlay();
	}
	if (ImGui::Button(u8"現在のステージでPlay"))
	{
		StagePlay();
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"削除"))
	{
		StageDataManager::GetInstance().RemoveStageData(mStageNameText);
	}
	ImGui::End();
}

void StageCreateScene::RuleImGuiDraw()
{
	ImGui::SetNextWindowPos(ImVec2(20, 400), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 150), ImGuiCond_Once);
	ImGui::Begin(u8"ゲームルール");
	ImGui::Text(u8"揃える例の面(数字)");
	ImGui::SliderInt("TargetDiceType", &mStageData->mTargetDiceType, 1, 6);
	ImGui::Text(u8"STEP数");
	ImGui::SliderInt("step", &mStageData->mStep, 1, 30);
	ImGui::End();
}

void StageCreateScene::StageDataSave()
{
	StageData data = mStageData.At();

	data.mStageName = mStageNameText;
	data.mPlayerPos = mViewObjList[ePlayer]->GetComponent<Component::MapPos>()->GetMapPos();

	data.mDiceMtx.clear();
	data.mDiceMtx.shrink_to_fit();

	DiceManager::GetInstance()->SetCreateDiceMap();
	// サイコロの数とMtx記憶
	for (int z = 0; z < data.mMapSizeHeight; z++)
	{
		for (int x = 0; x < data.mMapSizeWidth; x++)
		{
			data.mMap[z][x] = NODICE;
			continue;
		}
	}
	for (int z = 0; z < data.mMapSizeHeight; z++)
	{
		for (int x = 0; x < data.mMapSizeWidth; x++)
		{
			Dix::wp<Dice> dice = DiceManager::GetInstance()->GetCreateDice(INT2(x, z));

			if (dice == NULL)
				continue;
			// 配列に入れる
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
	// ステージをロード
	if (StageDataManager::GetInstance().LoadStage(mStageNameText))
	{
		// そのステージのポインタを取得
		Dix::wp<StageData> p = StageDataManager::GetInstance().GetStageData(mStageNameText);

		mStageData.At() = p.At();
		mStageData->mStageName = "create/init";
		StageDataManager::GetInstance().SetCurrentStage(mStageData->mStageName);
		DiceManager::GetInstance()->DataCreate();
		mViewObjList[eDiceM]->Init();
		mStage->Init();
		mStage->CameraUpdate();
		mViewObjList[ePlayer]->GetComponent<Component::MapPos>()->SetMapPosMove(mStageData->mPlayerPos);
	}
}

void StageCreateScene::StageDataPlay()
{
	bool sts = StageDataManager::GetInstance().SetCurrentStage(mStageNameText);
	if (sts)
	{
		// シーン切り替え
		DiceManager::GetInstance()->Uninit();
		SceneManager::GetInstance()->SetGameMode(GameMode::ePuzzle);
		SceneManager::GetInstance()->SetNextScene("GameMain");

		if (mIsStagePlay)
			strcpy(mStageNameText, mBeforeStageName.c_str());
	}
	else
	{
		MessageBox(nullptr, "指定されたステージデータは存在しません", "error", MB_OK);
		return;
	}
}

void StageCreateScene::StagePlay()
{
	// プレイ用ステージに保存
	mBeforeStageName = mStageNameText;
	mStageData->mStageName = "create/play";
	strcpy(mStageNameText, mStageData->mStageName.c_str());

	if (mIsPlay)
		StageDataManager::GetInstance().RemoveStageData(mStageNameText);

	StageDataSave();
	StageDataPlay();
	mStage->Init();

	mIsStagePlay = true;

	if (!mIsPlay)
		mIsPlay = true;
}
