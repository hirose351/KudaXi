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

TitleScene::~TitleScene()
{
}

void TitleScene::SceneAfter()
{
}

void TitleScene::SceneInit()
{
	Skydome* skydome = new Skydome;

	myUI::Image* rogo = new myUI::Image;
	rogo->GetTransform()->SetScale(Float3(700, 650, 0));
	rogo->GetTransform()->SetPositionXYZ(Float3(350, 350, 0));
	rogo->AddComponent<Component::Quad2d>()->SetInfo("assets/image/title/rogo.png", XMFLOAT4(1, 1, 1, 1));

	myUI::Image* i = new myUI::Image;
	i->GetTransform()->SetScale(Float3(300, 300, 0));
	i->GetTransform()->SetPositionXYZ(Float3(700, 500, 0));
	i->AddComponent<Component::Quad2d>()->SetInfo("assets/image/title/player.png", XMFLOAT4(1, 1, 1, 1), 4, 2);

	myUI::ButtonGroup* bG = new myUI::ButtonGroup;
	bG->GetTransform()->SetPositionXYZ(Float3(950.0f, 300.0f, 0));
	bG->SetInitState("assets/image/title/button.png", 2, 1, 1, ButtonTransition::eColorTint, XMFLOAT2(950.0f, 300.0f), XMFLOAT2(5, 5), XMFLOAT2(300, 300), XMFLOAT2(400, 400), ButtonArrangement::eVertical);
	bG->SetInitSelectNum(0);
	bG->SetPosition(0, XMFLOAT2(950.0f, 300.0f));
	bG->SetPosition(1, XMFLOAT2(1050, 500));
	mpBg = bG;

	Effect::Fruit* f = new Effect::Fruit;
	f->GetComponent<Component::Billbord>()->SetColor(XMFLOAT4(1, 1, 1, 0.5f));
	mpFruit = f;

	mCameraLookat = { 0,0,0 };
	CCamera::GetInstance()->SetLookat(mCameraLookat);
	CCamera::GetInstance()->SetEye(Float3(0, 0, -100));
	CCamera::GetInstance()->CreateCameraMatrix();
}

void TitleScene::SceneUpdate()
{
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_SPACE))
	{
		static int num = 0;
		mpFruit->SetInit(Float3(0, 100, 0), num);
		num = (num + 1) % 6;
	}

	static bool a = false;

	if (a)
		return;
	if (!mpBg->GetIsPressed())
		return;
	if (mpBg->GetSelectNum() == 0)
	{
		//SceneManager::GetInstance()->changeScene<MaingameScene>("GameMain", 1000, false);
		SceneManager::GetInstance()->changeScene<MaingameScene>("GameMain");
		a = true;
	}
	if (mpBg->GetSelectNum() == 1)
		SceneManager::GetInstance()->SetGameEndFlg(true);
}

void TitleScene::SceneRender()
{
}

bool TitleScene::Dispose()
{
	return false;
}
