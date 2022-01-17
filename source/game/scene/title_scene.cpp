#include		"title_scene.h"
#include		"../gameobject/ui_image.h"
#include		"../component/quad2d_component.h"
#include		"../component/model_component.h"
#include		"../component/billbord_component.h"
#include		"../gameobject/skydome.h"
#include		"../../system/dx11/CDirectInput.h"
#include		"maingame_scene.h"

using namespace DirectX;

TitleScene::TitleScene()
{
}

void TitleScene::SceneAfter()
{
	mCameraLookat = { 0,0,0 };
	Camera::GetInstance()->SetLookat(mCameraLookat);
	Camera::GetInstance()->SetEye(Float3(0, 0, -100));
	Camera::GetInstance()->CreateCameraMatrix();
	mIsButtonPush = false;
}

void TitleScene::SceneInit()
{
	Dix::sp<Skydome> skydome;
	skydome.SetPtr(new Skydome);
	AddGameObject(skydome);

	Dix::sp<myUI::Image> rogo;
	rogo.SetPtr(new myUI::Image);
	rogo->GetTransform()->SetScale(Float3(700, 650, 0));
	rogo->GetTransform()->SetPositionXYZ(Float3(350, 350, 0));
	rogo->AddComponent<Component::Quad2d>()->SetInfo("assets/image/title/rogo.png", XMFLOAT4(1, 1, 1, 1));
	AddGameObject(rogo);

	Dix::sp<myUI::Image> i;
	i.SetPtr(new myUI::Image);
	i->GetTransform()->SetScale(Float3(300, 300, 0));
	i->GetTransform()->SetPositionXYZ(Float3(700, 500, 0));
	i->AddComponent<Component::Quad2d>()->SetInfo("assets/image/title/player.png", XMFLOAT4(1, 1, 1, 1), 4, 2);
	AddGameObject(i);

	Dix::sp<myUI::ButtonGroup> bG;
	bG.SetPtr(new myUI::ButtonGroup);
	bG->GetTransform()->SetPositionXYZ(Float3(950.0f, 300.0f, 0));
	bG->SetInitState("assets/image/title/button.png", 2, 1, 1, ButtonTransition::eColorTint, XMFLOAT2(5, 5), XMFLOAT2(300, 300), XMFLOAT2(400, 400), ButtonArrangement::eVertical);
	bG->SetInitSelectNum(0);
	bG->SetPosition(0, XMFLOAT2(950.0f, 300.0f));
	bG->SetPosition(1, XMFLOAT2(1050, 500));
	mpBg = bG;
	AddGameObject(bG);
}

void TitleScene::SceneUpdate()
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
		mpSceneManager->SetGameEndFlg(true);
}