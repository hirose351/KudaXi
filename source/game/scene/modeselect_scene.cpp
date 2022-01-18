#include	"modeselect_scene.h"
#include	"../gameobject/ui_button.h"
#include	"../../application.h"

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
	Dix::sp<myUI::ButtonGroup> bG;
	bG.SetPtr(new myUI::ButtonGroup);
	bG->GetTransform()->SetPositionXYZ(Float3(320, 500, 0));
	bG->SetInitState("assets/image/ui/mode_button.png", 3, 1, 1, ButtonTransition::eColorTint, XMFLOAT2(5, 5), XMFLOAT2(300, 300), XMFLOAT2(350, 350), ButtonArrangement::eHorizontal);
	bG->SetInitSelectNum(0);
	mpBg = bG;
	AddGameObject(bG);
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
	else if (mpBg->GetSelectNum() == 1)
	{
		mpSceneManager->SetNextScene("GameMain");
		mIsButtonPush = true;
	}
	mpBg->SetIsPressed(false);
}
