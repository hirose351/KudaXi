#pragma once
#include	"../manager/SceneManager.h"
#include	"XAudio2.h"

class SceneManager;
class GameObject;

class IScene : Uncopyable {
private:
	SceneManager*	mManager;

	std::vector<GameObject*> objectList;	// ゲームオブジェクトを全部まとめて管理できるオブジェクトリスト

	Quad		gQuadfadein;
	Quad		gQuadfadeout;
public:
	IScene();
	void SetSceneManager(SceneManager* _sm) {
		mManager = _sm;
	}

	virtual ~IScene() {}

	// 初期化
	virtual bool Init() = 0;
	// 更新
	virtual void Update();
	// 描画
	virtual void Render();
	// デバッグ
	virtual void ImguiDebug() = 0;
	// 終了
	virtual bool Dispose();

	virtual void updateFadeIn(double t);
	virtual void updateFadeOut(double t);

	virtual void drawFadeIn(double t);
	virtual void drawFadeOut(double t);
};