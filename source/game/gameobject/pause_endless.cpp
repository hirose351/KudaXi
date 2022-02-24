#include	"pause_endless.h"
#include	"../gameobject/ui_image.h"
#include	"../component/quad2d_component.h"
#include	"../component/easing_component.h"
#include	"../manager/scene_manager.h"
#include	"../manager/input_manager.h"
#include	"../state/gamemode_controller.h"
#include	"../../application.h"
#include	"ui_button.h"

using namespace myUI;
using namespace DirectX;

PauseEndless::PauseEndless() :GameObject(("PauseEndless"), ObjectType::eObstracle, false)
{
	Dix::sp<ButtonGroup> b;
	b.SetPtr(new ButtonGroup);
	b->GetTransform()->SetPositionXYZ(Float3(Application::CLIENT_WIDTH / 2.0f, 300, 0));
	b->SetInitState("assets/image/ui/pause_button.png", 3, 1, 3, ButtonTransition::eColorTint, XMFLOAT2(0, -100), XMFLOAT2(200, 200), XMFLOAT2(300, 300), ButtonArrangement::eVertical);
	b->SetParent(this);
	b->AddComponent<Component::Easing>();
	b->SetIsActive(false);
	mpButton = b;
	mpButton->SetInitSelectNum(0);
	SceneManager::GetInstance()->GetScene(mSceneKey)->AddGameObject(b);

	Dix::sp<myUI::Image> diceBg;
	diceBg.SetPtr(new myUI::Image);
	diceBg->AddComponent<Component::Easing>();
	Component::Quad2d* bgQuad = diceBg->AddComponent<Component::Quad2d>();
	bgQuad->SetInfo("", XMFLOAT4(1, 1, 1, 0.8f));
	bgQuad->SetDrawType(DrawType::eNoTex);
	bgQuad->SetOrderInLayer(99);
	diceBg->GetTransform()->SetScale(Float3(500, 300, 0));
	diceBg->GetTransform()->SetPosition(Float3(Application::CLIENT_WIDTH / 2.0f, 400, 0));
	mpButtonBg = diceBg;
	mpButtonBg->SetIsActive(false);
	SceneManager::GetInstance()->GetScene(mSceneKey)->AddGameObject(diceBg);
}

void PauseEndless::ObjectInit()
{
	mpButton->SetIsActive(false);
	mpButtonBg->SetIsActive(false);
}

void PauseEndless::ObjectUpdate()
{
	// ポーズボタンが押されたら
	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::ePause)))
	{
		if (!mIsPause)
		{
			mIsPause = true;
			SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(true);
			mpButton->SetIsActive(true);
			mpButton->SetSelectedNum(0);
			mpButton->GetTransform()->SetPosition(Float3(-100, 300, 0));
			mpButton->Update();
			mpButton->GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eElasticOut, TransType::ePos, 20, 0, Float3(-100, 300, 0), Float3(Application::CLIENT_WIDTH / 2.0f, 300, 0));
			mpButtonBg->SetIsActive(true);
			mpButtonBg->Update();
		}
		else
		{
			// ポーズ中ならポーズ解除
			mIsPause = false;
			SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(false);
			mpButton->SetIsActive(false);
			mpButtonBg->SetIsActive(false);
		}
	}

	//if (isPause)
	//{
	//	// ポーズ状態で戻るボタンが押されたら
	//	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::eCancel)))
	//	{
	//		// ポーズ解除
	//		isPause = false;
	//		SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(false);
	//		mpButton->SetIsActive(false);
	//		mpButtonBg->SetIsActive(false);
	//	}
	//}

	if (mpButton->GetIsPressed())
	{
		if (mpButton->GetSelectNum() == 0)
		{
			mIsPause = false;
			SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(false);
			mpButton->SetIsActive(false);
			mpButtonBg->SetIsActive(false);
		}
		if (mpButton->GetSelectNum() == 1)
		{
			mpParent->GetComponent<Component::GameModeController>()->CurrentAfterChange();
			SceneManager::GetInstance()->SetNextScene("Mode");
		}
		if (mpButton->GetSelectNum() == 2)
		{
			mpParent->GetComponent<Component::GameModeController>()->CurrentAfterChange();
			SceneManager::GetInstance()->SetNextScene("Title");
		}
		mpButton->SetIsPressed(false);
	}
}
