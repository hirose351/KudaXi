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
	// シーン保管
	std::unordered_map<std::string, std::unique_ptr<SceneBase>>	mScenefactories;
	std::string mBeforescenekey;			// 前シーンのキー
	std::string mCurrentscenekey;			// カレントシーンのキー
	std::string mNextscenekey;				// 次シーン
	std::string mAddkey;					// 追加シーン

	bool mGameEndFlg = false;										// ゲームを終了するか

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
	// シーンを変更する1
	template <class sceneclass>
	void changeScene(std::string key);

	// カレントシーンをセットする
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

// シーンを変更する
template <class sceneclass>
void SceneManager::changeScene(std::string key) {

	// 指定されたキーが存在しているか
	auto it = mScenefactories.find(key);
	if (it != mScenefactories.end())
	{
		// 存在していた場合
		mScenefactories[key].get()->Init();
		mScenefactories[key].get()->SceneAfter();
		mScenefactories[key].get()->SetSceneManager(this);
	}
	else
	{
		// 存在していなかった場合生成
		mScenefactories[key] = std::move(std::make_unique<sceneclass>());
		mScenefactories[key].get()->Init();
		mScenefactories[key].get()->SetSceneManager(this);
	}
	// カレントシーンセット	
	SetCurrentScene(key);
}