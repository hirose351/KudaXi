#pragma once
#include	<string>
#include	<unordered_map>
#include	<functional>
#include	<iostream>
#include	<memory>
#include	<directxmath.h>
#include	<chrono>
#include	<thread>
#include	"../../system/util/uncopyable.h"
#include	"../scene/scene_base.h"

class SceneBase;

class SceneManager :public Uncopyable {
private:
	// �V�[���ۊ�
	std::unordered_map<std::string, std::unique_ptr<SceneBase>>	mScenefactories;
	std::string mBeforescenekey;			// �O�V�[���̃L�[
	std::string mCurrentscenekey;			// �J�����g�V�[���̃L�[
	std::string mNextscenekey;				// ���V�[��
	std::string mAddkey;					// �ǉ��V�[��

	bool mGameEndFlg = false;										// �Q�[�����I�����邩

public:

	static SceneManager* GetInstance() {
		static SceneManager Instance;
		return &Instance;
	}

	bool GetGameEndFlg() {
		return mGameEndFlg;
	}
	void SetGameEndFlg(bool _flg) {
		mGameEndFlg = _flg;
	}

	template <class sceneclass>
	void add(std::string key);
	// �V�[����ύX����1
	template <class sceneclass>
	void changeScene(std::string key);

	// �J�����g�V�[�����Z�b�g����
	void SetCurrentScene(std::string key);
	void Update();
	void Render();

	std::string GetCurrentSceneKey();
	std::string GetBeforeSceneKey();
	std::string GetAddSceneKey();

	SceneBase* GetCurrentScene() {
		return mScenefactories[mCurrentscenekey].get();
	}
	SceneBase* GetAddScene() {
		return mScenefactories[mAddkey].get();
	}
	SceneBase* GetScene(std::string _key) {
		return mScenefactories[_key].get();
	}
};

template <class sceneclass>
void SceneManager::add(std::string key) {
	mScenefactories[key] = std::move(std::make_unique<sceneclass>());
	mAddkey = key;
	mCurrentscenekey = key;
	mScenefactories[key].get()->SceneInit();
	mScenefactories[key].get()->Init();
	mScenefactories[key].get()->SetSceneManager(this);
}

// �V�[����ύX����
template <class sceneclass>
void SceneManager::changeScene(std::string key) {

	// �w�肳�ꂽ�L�[�����݂��Ă��邩
	auto it = mScenefactories.find(key);
	if (it != mScenefactories.end())
	{
		// ���݂��Ă����ꍇ
		mScenefactories[key].get()->Init();
		mScenefactories[key].get()->SceneAfter();
		mScenefactories[key].get()->SetSceneManager(this);
	}
	else
	{
		// ���݂��Ă��Ȃ������ꍇ����
		mScenefactories[key] = std::move(std::make_unique<sceneclass>());
		mScenefactories[key].get()->Init();
		mScenefactories[key].get()->SetSceneManager(this);
	}
	// �J�����g�V�[���Z�b�g	
	SetCurrentScene(key);
}