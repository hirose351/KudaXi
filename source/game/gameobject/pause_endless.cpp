#include	"pause_endless.h"
#include	"../component/quad2d_component.h"
#include	"../component/easing_component.h"
#include	"../manager/scene_manager.h"
#include	"../manager/input_manager.h"

using namespace myUI;
using namespace DirectX;

PauseEndless::PauseEndless() :GameObject(("PauseEndless"), ObjectType::eObstracle, false)
{
	Dix::sp<ButtonGroup> b;
	b.SetPtr(new ButtonGroup);
	b->GetTransform()->SetPositionXYZ(Float3(600, 300, 0));
	b->SetInitState("assets/image/ui/pause_button.png", 3, 1, 3, ButtonTransition::eColorTint, XMFLOAT2(0, -100), XMFLOAT2(200, 200), XMFLOAT2(300, 300), ButtonArrangement::eVertical);
	b->SetParent(this);
	b->AddComponent<Component::Easing>();
	b->SetIsActive(false);
	mButton = b;
	mButton->SetInitSelectNum(0);
	SceneManager::GetInstance()->GetScene(mSceneKey)->AddGameObject(b);
}

void PauseEndless::ObjectInit()
{
	mButton->SetIsActive(false);
}

void PauseEndless::ObjectUpdate()
{
	// ポーズボタンが押されたら
	if (InputManager::GetInstance().GetStateTrigger(InputMode::eUi, static_cast<int>(UiAction::ePause)))
	{
		if (!isPause)
		{
			isPause = true;
			SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(true);
			mButton->SetIsActive(true);
			mButton->SetSelectedNum(0);
			mButton->GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eExpIn, TransType::ePos, 300, 0, Float3(-100, 300, 0), Float3(600, 400, 0));
		}
		else
		{
			isPause = false;
			SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(false);
			mButton->SetIsActive(false);
		}
	}

	if (mButton->GetIsPressed())
	{
		if (mButton->GetSelectNum() == 0)
		{
			isPause = false;
			SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(false);
			mButton->SetIsActive(false);
		}
		if (mButton->GetSelectNum() == 1)
			SceneManager::GetInstance()->SetNextScene("Mode");
		if (mButton->GetSelectNum() == 2)
			SceneManager::GetInstance()->SetNextScene("Title");
		mButton->SetIsPressed(false);
	}
}
