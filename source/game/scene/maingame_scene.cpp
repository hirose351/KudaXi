#include	"maingame_scene.h"
#include	"../gameobject/gameobject.h"
#include	"../gameobject/player.h"
#include	"../gameobject/stage.h"
#include	"../gameobject/skydome.h"
#include	"../gameobject/ui_image.h"
#include	"../state/gamemode_controller.h"
#include	"../component/quad2d_component.h"
#include	"../component/easing_component.h"

using namespace Dix;

MaingameScene::MaingameScene()
{
}

void MaingameScene::SceneAfter()
{
}

void MaingameScene::SceneInit()
{
	Dix::sp<Player> player;
	player.SetPtr(new Player);
	AddGameObject(player);

	Dix::sp<Stage> stage;
	stage.SetPtr(new Stage);
	AddGameObject(stage);

	Dix::sp<Skydome> skydome;
	skydome.SetPtr(new Skydome);
	AddGameObject(skydome);

	// •¶Žš
	Dix::sp<myUI::Image> backUiDice;
	backUiDice.SetPtr(new myUI::Image);
	backUiDice->GetTransform()->SetScale(Float3(150));
	Component::Quad2d* uiStageQuad = backUiDice->AddComponent<Component::Quad2d>();
	uiStageQuad->SetInfo("assets/image/ui/back.png", XMFLOAT4(1, 1, 1, 0.8f), 2);
	uiStageQuad->SetOrderInLayer(10);
	backUiDice->GetTransform()->SetPositionXYZ(Float3(90, 630.0f, 0));
	AddGameObject(backUiDice);

	Dix::sp<myUI::Image> backUiArrow;
	backUiArrow.SetPtr(new myUI::Image);
	backUiArrow->SetIsStopPause(false);
	backUiArrow->GetTransform()->SetScale(Float3(150));
	uiStageQuad = backUiArrow->AddComponent<Component::Quad2d>();
	uiStageQuad->SetInfo("assets/image/ui/back.png", XMFLOAT4(1, 1, 1, 0.7f), 2);
	uiStageQuad->SetOrderInLayer(11);
	uiStageQuad->SetUvPos(INT2(1, 0));
	backUiArrow->GetTransform()->SetPositionXYZ(Float3(90, 630.0f, 0));
	backUiArrow->AddComponent<Component::Easing>()->SetIsRepeat(true);
	backUiArrow->GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::ePos, 70, 0, Float3(0, 0, 0), Float3(95, 630.0f, 0), true);
	backUiArrow->GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::ePos, 70, 0, Float3(0, 0, 0), Float3(85, 630.0f, 0), true);
	AddGameObject(backUiArrow);

	Dix::sp<GameObject> maingameController;
	maingameController.SetPtr(new GameObject(("GameModeController"), ObjectType::eObstracle, false));
	maingameController->AddComponent<Component::GameModeController>()->SetPlayer(player);
	maingameController->GetComponent<Component::GameModeController>()->SetStage(stage);
	AddGameObject(maingameController);
}

void MaingameScene::SceneUpdate()
{
}