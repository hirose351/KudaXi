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
	mButton->SetInitState("assets/image/ui/pause_button.png", 3, 1, 1, ButtonTransition::eColorTint, XMFLOAT2(600, 300), XMFLOAT2(20, 20), XMFLOAT2(200, 200), XMFLOAT2(300, 300));
	mButton->SetIsActive(false);
}

void PauseEndless::ObjectInit()
{

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
		}
		else
		{
			isPause = false;
			SceneManager::GetInstance()->GetCurrentScene()->SetIsPause(false);
			mButton->SetIsActive(false);
		}
	}
}
