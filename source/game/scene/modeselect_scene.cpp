#include	"modeselect_scene.h"
#include	"../gameobject/ui_button.h"
#include	"../../application.h"
#include	"../gameobject/skydome.h"
#include	"../gameobject/ui_image.h"
#include	"../component/quad2d_component.h"
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
