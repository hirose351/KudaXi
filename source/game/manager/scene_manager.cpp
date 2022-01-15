#include	"scene_manager.h"

// カレントシーンをセットする
void SceneManager::setCurrentScene(std::string key) {

	mBeforescenekey = mCurrentscenekey;
	mCurrentscenekey = key;
	mAddkey = mCurrentscenekey;
	mScenefactories[mCurrentscenekey]->SceneAfter();
}

void SceneManager::Update() {

	mScenefactories[mCurrentscenekey]->Update();
}

void SceneManager::Render() {

	// カレントシーンキーが空なら何もしない
	if (mCurrentscenekey.empty())
	{
		return;
	}
	// カレントシーン描画
	mScenefactories[mCurrentscenekey]->Render();
}

std::string SceneManager::GetCurrentSceneKey() {
	return mCurrentscenekey;
}

std::string SceneManager::GetBeforeSceneKey()
{
	return mBeforescenekey;
}

std::string SceneManager::GetAddSceneKey()
{
	return mAddkey;
}