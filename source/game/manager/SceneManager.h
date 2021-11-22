#pragma once
#include	<string>
#include	<unordered_map>
#include	<functional>
#include	<iostream>
#include	<memory>
#include	<directxmath.h>
#include	<chrono>
#include	<thread>
#include	"../../Application.h"
#include	"../../system/util/uncopyable.h"
#include	"../../system/util/StopWatch.h"
#include	"../../system/util/quad2d.h"
#include	"../scene/scene_base.h"

class SceneBase;

class SceneManager :public Uncopyable {
private:
	// 遷移方法定義列挙クラス
	enum class TransitionState
	{
		None,
		FadeIn,
		Active,
		FadeOut,
		FadeInOut,
	};

	// シーン保管
	std::unordered_map<std::string, std::unique_ptr<SceneBase>>	mScenefactories;
	std::string mBeforescenekey;			// 前シーンのキー
	std::string mCurrentscenekey;			// カレントシーンのキー
	std::string mNextscenekey;				// 次シーン

	// 画面遷移法
	TransitionState mTransitionState = TransitionState::None;

	int32_t	mTransitionTimeMillisec = 1000;	// 遷移時間（デフォルト：１０００ミリ秒）

	Stopwatch mStopwatch;						// ストップウオッチ

	DirectX::XMFLOAT4 mFadeColor = DirectX::XMFLOAT4(0, 0, 0, 1);		// フェードカラー

	bool mCrossFade = false;

	bool mGameEndFlg = false;	// ゲームを終了するか

	void UpdateSingle();					// ひとつの遷移方法で更新
	void UpdateCross();						// クロスフェードで更新
	void DrawScene();						// カレントシーン描画
	void SetTransitionState(SceneManager::TransitionState ts);	// 遷移方法セット

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

	// シーンを変更する2
	template <class sceneclass>
	bool changeScene(std::string key, int32_t transitionTimeMillisec, bool crossFade);

	// カレントシーンをセットする
	void setCurrentScene(std::string key);
	void Update();
	void Render();

	std::string GetCurrentSceneKey();
	std::string GetBeforeSceneKey();
	DirectX::XMFLOAT4 GetFadeColor();
	void SetFadeColor(DirectX::XMFLOAT4 color);
};

template <class sceneclass>
void SceneManager::add(std::string key) {
	mScenefactories[key] = std::move(std::make_unique<sceneclass>());
	mScenefactories[key].get()->Init();
	mScenefactories[key].get()->SetSceneManager(this);

	SetTransitionState(SceneManager::TransitionState::Active);
}

// シーンを変更する
template <class sceneclass>
void SceneManager::changeScene(std::string key) {

	// 指定されたキーが存在しているか
	auto it = mScenefactories.find(key);
	if (it != mScenefactories.end())
	{
		// 存在していた場合
		mScenefactories[key].get()->Init(this);
		mScenefactories[key].get()->SetSceneManager(this);
	}
	else
	{
		// 存在していなかった場合生成
		mScenefactories[key] = std::move(std::make_unique<sceneclass>());
		mScenefactories[key].get()->Init(this);
		mScenefactories[key].get()->SetSceneManager(this);
	}
	// カレントシーンセット	
	setCurrentScene(key);
}

template <class sceneclass>
bool SceneManager::changeScene(std::string key, int32_t transitionTimeMillisec, bool crossFade) {
	// 前のシーンをカレントシーンにする
	mBeforescenekey = mCurrentscenekey;

	// 現在のシーンと変更後のシーンが同じであればクロスフェード行わない
	if (key == mCurrentscenekey)
	{
		crossFade = false;
	}
	// 指定されたキーが存在しているか
	auto it = mScenefactories.find(key);
	if (it != mScenefactories.end())
	{
		// 存在していた場合
		// シーン初期化
		mScenefactories[key].get()->Init();
		// シーンマネージャのポインタをセット
		mScenefactories[key].get()->SetSceneManager(this);
	}
	else
	{
		// 存在していなかった場合生成
		mScenefactories[key] = std::move(std::make_unique<sceneclass>());
		// シーン初期化
		mScenefactories[key].get()->Init();
		// シーンマネージャのポインタをセット
		mScenefactories[key].get()->SetSceneManager(this);
	}
	mNextscenekey = key;
	mCrossFade = crossFade;
	// クロスフェードを行う
	if (crossFade)
	{
		// 遷移時間をメンバ変数にセット
		mTransitionTimeMillisec = transitionTimeMillisec;
		mTransitionState = TransitionState::FadeInOut;
		mStopwatch.restart();
	}
	else
	{
		//　フェードアウト後にフェードインなので遷移時間を2で割る
		mTransitionTimeMillisec = (transitionTimeMillisec / 2);
		mTransitionState = TransitionState::FadeOut;
		mStopwatch.restart();
	}

	return true;
}