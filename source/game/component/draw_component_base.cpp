#include		"draw_component_base.h"
#include		"../manager/scene_manager.h"

DrawComponentBase::DrawComponentBase() :ComponentBase(("DrawBase")), mOrderInLayer(0), mIsDraw(true)
{
	mSceneName = SceneManager::GetInstance()->GetCurrentSceneKey();
	// マネージャーに自身を登録
	SceneManager::GetInstance()->GetScene(mSceneName)->AddDrawComponent(this);
}

DrawComponentBase::~DrawComponentBase()
{
	if (SceneManager::GetInstance()->GetScene(mSceneName) == nullptr)
		return;
	SceneManager::GetInstance()->GetScene(mSceneName)->RemoveDrawComponent(this);
}

void DrawComponentBase::SetDrawPos(Float3 _f3)
{
	mDrawPosList.emplace_back(_f3);
}