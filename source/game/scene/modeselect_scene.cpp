#include	"modeselect_scene.h"
#include	"../gameobject/ui_button.h"
#include	"../../application.h"
#include	"../gameobject/skydome.h"
#include	"../gameobject/ui_image.h"
#include	"../component/quad2d_component.h"
#include	"../component/easing_component.h"
#include	"../manager/input_manager.h"
#include	"../../system/util/XAudio2.h"

using namespace DirectX;

enum class SelectMode
{
	eTutorial,
	ePuzzle,
	eEndless,
	eEdit,
};

ModeSelectScene::ModeSelectScene()
{
}

void ModeSelectScene::SceneAfter()
{
	if (mpSceneManager->GetBeforeSceneKey() == "Create")
	{
		StopSound(SOUND_LABEL_BGM_CREATE);
		PlaySound(SOUND_LABEL_BGM_TITLE);
	}
}

void ModeSelectScene::SceneInit()
{
	Dix::sp<myUI::ButtonGroup> buttonG;
	buttonG.SetPtr(new myUI::ButtonGroup);
	buttonG->GetTransform()->SetPositionXYZ(Float3(200, 500, 0));
	buttonG->SetInitState("assets/image/ui/mode_button.png", 4, 1, 4, ButtonTransition::eColorTint, XMFLOAT2(5, 5), XMFLOAT2(300, 300), XMFLOAT2(350, 350), ButtonArrangement::eHorizontal);
	buttonG->SetInitSelectNum(0);
	mpBg = buttonG;
	AddGameObject(buttonG);

	Dix::sp<Skydome> skydome;
	skydome.SetPtr(new Skydome);
	skydome->SetType(Skydome::Type::TITLE);
	AddGameObject(skydome);

	Dix::sp<myUI::Image> modeUi;
	modeUi.SetPtr(new myUI::Image);
	modeUi->GetTransform()->SetScale(Float3(1000, 150, 0));
	modeUi->GetTransform()->SetPositionXYZ(Float3(600, 150, 0));
	modeUi->AddComponent<Component::Quad2d>()->SetInfo("assets/image/ui/mode.png", XMFLOAT4(1, 1, 1, 1));
	AddGameObject(modeUi);

	// •¶Žš
	Dix::sp<myUI::Image> backUiDice;
	backUiDice.SetPtr(new myUI::Image);
	backUiDice->GetTransform()->SetScale(Float3(120));
	Component::Quad2d* uiStageQuad = backUiDice->AddComponent<Component::Quad2d>();
	uiStageQuad->SetInfo("assets/image/ui/back.png", XMFLOAT4(1, 1, 1, 0.8f), 2);
	uiStageQuad->SetOrderInLayer(10);
	backUiDice->GetTransform()->SetPositionXYZ(Float3(80, 640.0f, 0));
	AddGameObject(backUiDice);

	Dix::sp<myUI::Image> backUiArrow;
	backUiArrow.SetPtr(new myUI::Image);
	backUiArrow->SetIsStopPause(false);
	backUiArrow->GetTransform()->SetScale(Float3(120));
	uiStageQuad = backUiArrow->AddComponent<Component::Quad2d>();
	uiStageQuad->SetInfo("assets/image/ui/back.png", XMFLOAT4(1, 1, 1, 0.7f), 2);
	uiStageQuad->SetOrderInLayer(11);
	uiStageQuad->SetUvPos(INT2(1, 0));
	backUiArrow->GetTransform()->SetPositionXYZ(Float3(80, 640.0f, 0));
	backUiArrow->AddComponent<Component::Easing>()->SetIsRepeat(true);
	backUiArrow->GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::ePos, 70, 0, Float3(0, 0, 0), Float3(85, 640.0f, 0), true);
	backUiArrow->GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eLinear, TransType::ePos, 70, 0, Float3(0, 0, 0), Float3(75, 640.0f, 0), true);
	AddGameObject(backUiArrow);
}

void ModeSelectScene::SceneUpdate()
{
	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eCancel)))
	{
		SceneManager::GetInstance()->SetNextScene("Title");
	}

	if (!mpBg->GetIsPressed())
		return;

	switch (static_cast<SelectMode>(mpBg->GetSelectNum()))
	{
	case SelectMode::eTutorial:
	{
	}
	break;
	case SelectMode::ePuzzle:
	{
		mpSceneManager->SetGameMode(GameMode::eSelect);
		mpSceneManager->SetNextScene("GameMain");
	}
	break;
	case SelectMode::eEndless:
	{
		mpSceneManager->SetGameMode(GameMode::eEndless);
		mpSceneManager->SetNextScene("GameMain");
	}
	break;
	case SelectMode::eEdit:
	{
		mpSceneManager->SetGameMode(GameMode::eEdit);
		mpSceneManager->SetNextScene("Create");
	}
	break;
	default:
		break;
	}

	mpBg->SetIsPressed(false);
}
