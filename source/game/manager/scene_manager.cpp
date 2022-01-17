#include	"scene_manager.h"

// �J�����g�V�[�����Z�b�g����
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

	// �J�����g�V�[���L�[����Ȃ牽�����Ȃ�
	if (mCurrentSceneKey.empty())
		return;

	mScenefactories[mCurrentSceneKey]->Update();
}

void SceneManager::Render() {

	// �J�����g�V�[���L�[����Ȃ牽�����Ȃ�
	if (mCurrentSceneKey.empty())
		return;

	// �J�����g�V�[���`��
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