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
	mSelectObjNum = 0;
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
		DiceManager::GetInstance()->SetIsSelect(false);
	}
	else
	{
		mViewObjList[0]->GetComponent<Component::Collision>()->SetIsDraw(false);
		mViewObjList[0]->GetComponent<Component::MapMove>()->SetState(ObjectState::ePaused);
		DiceManager::GetInstance()->SetIsSelect(true);
	}
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
	if (ImGui::Button(u8"現在のステージをPlay"))
	{
		/// Todo:処理作る
	}
	ImGui::End();
}

void StageCreateScene::RuleImGuiDraw()
{
	ImGui::SetNextWindowPos(ImVec2(20, 400), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 150), ImGuiCond_Once);
	ImGui::Begin(u8"ゲームルール");
	ImGui::Text(u8"揃える例の面(数字)");
	ImGui::SliderInt("TargetDiceType", &mStage.mTargetDiceType, 1, 6);
	ImGui::Text(u8"STEP数");
	ImGui::SliderInt("step", &mStage.mStep, 1, 30);
	ImGui::End();
}

void StageCreateScene::StageDataSave()
{
	mStage.mStageName = mStageNameText;
	mStage.mPlayerPos = mViewObjList[0]->GetComponent<Component::MapPos>()->GetMapPos();

	mStage.mDiceMtx.clear();
	mStage.mDiceMtx.shrink_to_fit();

	// サイコロの数とMtx記憶
	for (int z = 0; z < mStage.mMapSizeHeight; z++)
	{
		for (int x = 0; x < mStage.mMapSizeWidth; x++)
		{
			Dix::wp<Dice> dice = DiceManager::GetInstance()->GetCreateDice(INT2(x, z));

			if (dice == NULL)
			{
				mStage.mMap[z][x] = NODICE;
				continue;
			}
			// 配列に入れる
			mStage.mDiceMtx.emplace_back();
			mStage.mDiceMtx[mStage.mDiceMtx.size() - 1] = dice->GetTransform()->GetMtx();
			mStage.mMap[z][x] = dice->GetObjectID();

		}
	}
	StageDataManager::GetInstance().SaveStage(mStage);
}

void StageCreateScene::StageDataLoad()
{
	// ステージをロード
	if (StageDataManager::GetInstance().LoadStage(mStageNameText))
	{
		// そのステージのポインタを取得
		Dix::wp<StageData> p = StageDataManager::GetInstance().GetStageData(mStageNameText);

		mStage = p.At();
		StageDataManager::GetInstance().SetCurrentStage(mStage.mStageName);
		DiceManager::GetInstance()->DataCreate();
		mViewObjList[eDiceM]->Init();
		mViewObjList[ePlayer]->GetComponent<Component::MapPos>()->SetMapPos(mStage.mPlayerPos);
	}
}

void StageCreateScene::StageDataPlay()
{
	bool sts = StageDataManager::GetInstance().SetCurrentStage(mStageNameText);
	if (sts)
	{
		// シーン切り替え
		StopSound(SOUND_LABEL_BGM_CREATE);
		PlaySound(SOUND_LABEL_BGM_GAME);
		SceneManager::GetInstance()->SetNextScene("GameMain");
	}
	else
	{
		MessageBox(nullptr, "指定されたステージデータは存在しません", "error", MB_OK);
		return;
	}
}