#pragma once
#include	"../manager/SceneManager.h"
#include	"XAudio2.h"

class SceneManager;

class IScene : Uncopyable {
private:
	SceneManager*	mManager;
public:
	IScene();
	void SetSceneManager(SceneManager* _sm) {
		mManager = _sm;
	}

	virtual ~IScene() {}

	// 初期化
	virtual void Init() = 0;
	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Render() = 0;
	// デバッグ
	virtual void imguidebug() = 0;
	// 終了
	virtual void Dispose() = 0;

	virtual void updateFadeIn(double t);
	virtual void updateFadeOut(double t);

	virtual void drawFadeIn(double t);
	virtual void drawFadeOut(double t);
};