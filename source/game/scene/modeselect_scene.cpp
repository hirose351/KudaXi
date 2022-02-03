#include	"modeselect_scene.h"
#include	"../gameobject/ui_button.h"
#include	"../../application.h"
#include	"../gameobject/skydome.h"
#include	"../gameobject/ui_image.h"
#include	"../component/quad2d_component.h"

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
	mIsButtonPush = false;
}

void ModeSelectScene::SceneInit()
{
	Dix::sp<myUI::ButtonGroup> buttonG;
	buttonG.SetPtr(new myUI::ButtonGroup);
	buttonG->GetTransform()->SetPositionXYZ(Float3(200, 500, 0));
	buttonG->SetInitState("assets/image/ui/mode_button.png", 4, 1, 1, ButtonTransition::eColorTint, XMFLOAT2(5, 5), XMFLOAT2(300, 300), XMFLOAT2(350, 350), ButtonArrangement::eHorizontal);
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
	if (mIsButtonPush)
		return;
	if (!mpBg->GetIsPressed())
		return;


	switch (static_cast<SelectMode>(mpBg->GetSelectNum()))
	{
	case SelectMode::eTutorial:
	{
		mpSceneManager->SetNextScene("GameMain");
		mIsButtonPush = true;
	}
	break;
	case SelectMode::ePuzzle:
	{
		mpSceneManager->SetNextScene("GameMain");
		mpSceneManager->SetGameMode(GameMode::eSelect);
		mIsButtonPush = true;
	}
	break;
	case SelectMode::eEndless:
	{
		mpSceneManager->SetNextScene("GameMain");
		mIsButtonPush = true;
	}
	break;
	case SelectMode::eEdit:
	{
		mpSceneManager->SetNextScene("Create");
		mIsButtonPush = true;
	}
	break;
	default:
		break;
	}

	mpBg->SetIsPressed(false);
}
