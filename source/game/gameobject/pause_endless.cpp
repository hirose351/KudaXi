#include	"pause_endless.h"
#include	"../component/quad2d_component.h"
#include	"../component/easing_image_component.h"
#include	"../manager/scene_manager.h"

using namespace myUI;

PauseEndless::PauseEndless() :GameObject(("PauseEndless"), ObjectType::eObstracle, false)
{
	mButton = new ButtonGroup;
	mButton->SetInitState("assets/image/ui/pause_button.png", 3, 1, 1, ButtonTransition::eColorTint, XMFLOAT2(100, 300), XMFLOAT2(0, 0), XMFLOAT2(100, 100), XMFLOAT2(150, 150));
}

void PauseEndless::ObjectInit()
{

}

void PauseEndless::ObjectUpdate()
{
	// ƒ|[ƒY’†‚È‚ç
	if (!SceneManager::GetInstance()->GetCurrentScene()->GetIsPause())
	{
		if (isPause)
		{
			mButton->SetObjectState(ObjectState::ePaused);
			isPause = false;
		}
		return;
	}
	// 

}
