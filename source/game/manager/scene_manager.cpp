#include	"scene_manager.h"

// �J�����g�V�[�����Z�b�g����
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

	// �J�����g�V�[���L�[����Ȃ牽�����Ȃ�
	if (mCurrentscenekey.empty())
	{
		return;
	}
	// �J�����g�V�[���`��
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