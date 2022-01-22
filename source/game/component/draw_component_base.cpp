#include		"draw_component_base.h"
#include		"../manager/scene_manager.h"

std::string sceneName;

DrawComponentBase::DrawComponentBase() :ComponentBase(("DrawBase")), mOrderInLayer(0), mIsDraw(true)
{
	// マネージャーに自身を登録
	sceneName = SceneManager::GetInstance()->GetCurrentSceneKey();
	SceneManager::GetInstance()->GetScene(sceneName)->AddDrawComponent(this);
}

DrawComponentBase::~DrawComponentBase()
{
	if (SceneManager::GetInstance()->GetScene(sceneName) == nullptr)
		return;
	SceneManager::GetInstance()->GetScene(sceneName)->RemoveDrawComponent(this);
}

void DrawComponentBase::SetDrawPos(Float3 _f3)
{
	mDrawPosList.emplace_back(_f3);
}
