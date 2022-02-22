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
#include	"../gameobject/gameobject_utility.h"

class SceneBase;

class SceneManager :public Uncopyable {
private:
	std::unordered_map<std::string, std::unique_ptr<SceneBase>>	mScenefactories;	// シーン保管
	std::string mBeforeSceneKey = "";		// 前シーンのキー
	std::string mCurrentSceneKey = "";		// カレントシーンのキー
	std::string mNextSceneKey = "";			// 次シーン
	std::string mAddkey = "";				// 追加シーン

	bool mGameEndFlg = false;				// ゲームを終了するか
	GameMode mGameMode;
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
	void AddScene(std::string key);

	// カレントシーンをセットする
	void SetCurrentScene(std::string key);
	void SetNextScene(std::string key);
	void ChangeNextScene();
	void Update();
	void Render();

	std::string GetCurrentSceneKey();
	std::string GetBeforeSceneKey();
	std::string GetAddSceneKey();

	SceneBase* GetCurrentScene() {
		return mScenefactories[mCurrentSceneKey].get();
	}
	SceneBase* GetAddScene() {
		return mScenefactories[mAddkey].get();
	}
	SceneBase* GetScene(std::string _key) {
		if (mScenefactories.empty())
			return nullptr;
		return mScenefactories[_key].get();
	}

	GameMode GetGameMode() { return mGameMode; };
	void SetGameMode(GameMode _gameMode) { mGameMode = _gameMode; };
};

template <class sceneclass>
void SceneManager::AddScene(std::string key) {
	mScenefactories[key] = std::move(std::make_unique<sceneclass>());
	mAddkey = key;
	mCurrentSceneKey = key;
	mScenefactories[key].get()->SceneInit();
	mScenefactories[key].get()->SetSceneManager(this);
}