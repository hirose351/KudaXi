#include	"scene_manager.h"

// カレントシーンをセットする
void SceneManager::SetCurrentScene(std::string key) {

	mBeforeSceneKey = mCurrentSceneKey;
	mCurrentSceneKey = key;
	mAddkey = mCurrentSceneKey;
	mScenefactories[mCurrentSceneKey]->Init();
	mScenefactories[mCurrentSceneKey]->SceneAfter();
}

void SceneManager::SetNextScene(std::string key)
{
	mScenefactories[mCurrentSceneKey].get()->DrawFadeIn();
	mNextSceneKey = key;
}

void SceneManager::ChangeNextScene()
{
	mBeforeSceneKey = mCurrentSceneKey;
	mCurrentSceneKey = mNextSceneKey;
	mAddkey = mCurrentSceneKey;
	mScenefactories[mCurrentSceneKey]->Init();
	mScenefactories[mCurrentSceneKey]->SceneAfter();
	mScenefactories[mNextSceneKey].get()->DrawFadeOut();
}

void SceneManager::Update() {

	// カレントシーンキーが空なら何もしない
	if (mCurrentSceneKey.empty())
		return;

	mScenefactories[mCurrentSceneKey]->Update();
}

void SceneManager::Render() {

	// カレントシーンキーが空なら何もしない
	if (mCurrentSceneKey.empty())
		return;

	// カレントシーン描画
	mScenefactories[mCurrentSceneKey]->Render();
}

std::string SceneManager::GetCurrentSceneKey() {
	return mCurrentSceneKey;
}

std::string SceneManager::GetBeforeSceneKey()
{
	return mBeforeSceneKey;
}

std::string SceneManager::GetAddSceneKey()
{
	return mAddkey;
}