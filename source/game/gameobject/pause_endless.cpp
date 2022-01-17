#include	"pause_endless.h"
#include	"../component/quad2d_component.h"
#include	"../component/easing_component.h"
#include	"../manager/scene_manager.h"
#include	"../manager/input_manager.h"

using namespace myUI;
using namespace DirectX;

PauseEndless::PauseEndless() :GameObject(("PauseEndless"), ObjectType::eObstracle, false)
{
	mButton = new ButtonGroup;
	mButton->GetTransform()->SetPositionXYZ(Float3(600, 300, 0));
	mButton->SetInitState("assets/image/ui/pause_button.png", 3, 1, 3, ButtonTransition::eColorTint, XMFLOAT2(0, -100), XMFLOAT2(200, 200), XMFLOAT2(300, 300), ButtonArrangement::eVertical);
	mButton->SetIsActive(false);
	mButton->SetParent(this);
	mButton->AddComponent<Component::Easing>();
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
			mButton->SetInitSelectNum(0);
			mButton->GetComponent<Component::Easing>()->AddEasing(EasingProcess::EasingType::eExpIn, TransType::ePos, 300, 0, XMFLOAT2(-100, 300), XMFLOAT2(600, 400));
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
		if (mButton->GetSelectNum() == 2)
			SceneManager::GetInstance()->SetNextScene("Title");
	}
}
