#include		"title_scene.h"
#include		"../gameobject/ui_image.h"
#include		"../component/quad2d_component.h"
#include		"../component/model_component.h"
#include		"../component/billbord_component.h"
#include		"../component/animation2d_component.h"
#include		"../gameobject/skydome.h"
#include		"../../system/dx11/CDirectInput.h"
#include		"maingame_scene.h"
#include		"../../system/util/XAudio2.h"

using namespace DirectX;

TitleScene::TitleScene()
{
	PlaySound(SOUND_LABEL_BGM_TITLE);
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

	Dix::sp<myUI::Image> logo;
	logo.SetPtr(new myUI::Image);
	logo->GetTransform()->SetScale(Float3(700, 650, 0));
	logo->GetTransform()->SetPositionXYZ(Float3(350, 350, 0));
	logo->AddComponent<Component::Quad2d>()->SetInfo("assets/image/title/rogo.png", XMFLOAT4(1, 1, 1, 1));
	AddGameObject(logo);

	Dix::sp<myUI::Image> i;
	i.SetPtr(new myUI::Image);
	i->GetTransform()->SetScale(Float3(300, 300, 0));
	i->GetTransform()->SetPositionXYZ(Float3(700, 500, 0));
	i->AddComponent<Component::Quad2d>()->SetInfo("assets/image/title/player.png", XMFLOAT4(1, 1, 1, 1), 4, 2);
	i->AddComponent<Component::Animation2d>()->SetAnim(10, 0);
	mpPlayerImage = i->GetComponent<Component::Animation2d>();
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
	// ボタンの位置に合わせてプレイヤー画像アニメーションを変更
	if (mpBg->GetSelectNum() == 0)
		mpPlayerImage->SetAnim(10, 0);
	else if (mpBg->GetSelectNum() == 1)
		mpPlayerImage->SetAnim(10, 1);

	if (mIsButtonPush)
		return;
	if (!mpBg->GetIsPressed())
		return;
	if (mpBg->GetSelectNum() == 0)
	{
		mpSceneManager->SetNextScene("Mode");
		mIsButtonPush = true;
	}
	else if (mpBg->GetSelectNum() == 1)
		mpSceneManager->SetGameEndFlg(true);
}