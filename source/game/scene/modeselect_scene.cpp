#include	"modeselect_scene.h"
#include	"../gameobject/ui_button.h"
#include	"../../application.h"
#include	"../gameobject/skydome.h"
#include	"../gameobject/ui_image.h"
#include	"../component/quad2d_component.h"

using namespace DirectX;

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
	buttonG->GetTransform()->SetPositionXYZ(Float3(320, 500, 0));
	buttonG->SetInitState("assets/image/ui/mode_button.png", 3, 1, 1, ButtonTransition::eColorTint, XMFLOAT2(5, 5), XMFLOAT2(300, 300), XMFLOAT2(350, 350), ButtonArrangement::eHorizontal);
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
	if (mpBg->GetSelectNum() == 0)
	{
		mpSceneManager->SetNextScene("GameMain");
		mIsButtonPush = true;
	}
	else if (mpBg->GetSelectNum() == 1)
	{
		mpSceneManager->SetNextScene("GameMain");
		mIsButtonPush = true;
	}
	else if (mpBg->GetSelectNum() == 2)
	{
		mpSceneManager->SetNextScene("Create");
		mIsButtonPush = true;
	}
	mpBg->SetIsPressed(false);
}
