#include	"stagecreate_scene.h"
#include	"../gameobject/gameobject.h"
#include	"../gameobject/dice.h"
#include	"../gameobject/stage.h"
#include	"../gameobject/skydome.h"
#include	"../gameobject/access_dice_manager.h"
#include	"../component/map_pos_component.h"
#include	"../component/map_move_component.h"
#include	"../component/collision_component.h"
#include	"../../application.h"
#include	"../../system/dx11/DX11util.h"

using namespace Dix;

StageCreateScene::StageCreateScene()
{
}

void StageCreateScene::SceneAfter()
{
}

void StageCreateScene::SceneInit()
{
	Dix::sp<GameObject> player;
	player.SetPtr(new GameObject("Player", ObjectType::ePlayer, false));
	player->AddComponent<Component::MapPos>();
	player->AddComponent<Component::MapMove>();
	player->AddComponent<Component::Model>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/player/player.fbx"));
	player->GetTransform()->SetPositionY(DICE_SCALE);
	player->GetTransform()->scale = Float3(4, 6, 4);
	player->AddComponent<Component::Collision>()->SetLocalScale(player->GetTransform()->scale);
	player->GetComponent<Component::Collision>()->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
	player->GetComponent<Component::Collision>()->SetOrderInLayer(30);
	AddGameObject(player);
	mViewObjList.emplace_back(player);

	Dix::sp<Stage> stage;
	stage.SetPtr(new Stage);
	AddGameObject(stage);

	Dix::sp<Skydome> skydome;
	skydome.SetPtr(new Skydome);
	skydome->SetType(Skydome::Type::CLEATE);
	AddGameObject(skydome);

	//Dix::sp<DiceManagerAccess> dicemanager;
	//dicemanager.SetPtr(new DiceManagerAccess);
	//AddGameObject(dicemanager);
}

void StageCreateScene::SceneUpdate()
{

}

void StageCreateScene::ImguiDebug()
{
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 300), ImGuiCond_Once);
	ImGui::Begin(u8"GameObject");
	for (auto &obj : mViewObjList)
	{
		ImGui::Text(obj->GetName().c_str());
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(900, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 300), ImGuiCond_Once);
	ImGui::Begin(u8"Inspector");
	ImGui::SetNextTreeNodeOpen(true, ImGuiCond_Once);
	for (auto &obj : mViewObjList)
	{
		obj->ImguiComponentDraw();
	}
	ImGui::End();
	/// SaveLoad ///////////////////////////////////////////////////////////////////////////////////////////

	ImGui::SetNextWindowPos(ImVec2(900, 400), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(350, 300), ImGuiCond_Once);
	ImGui::Begin(u8"SaveLoad");

	//ImGui::InputText("StageName", mStageNameText, sizeof(mStageNameText));
	if (ImGui::Button("Save"))
	{
		//StageDataSave();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		//StageDataLoad();
	}
	ImGui::SameLine();
	if (ImGui::Button("Play"))
	{
		//StageDataPlay();
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove"))
	{
		//StageManager::GetInstance().RemoveStageData(mStageNameText);
	}
	ImGui::End();
}
