#pragma once
#include	"../../system/util/vector.h"
#include	"../manager/SceneManager.h"
#include	"XAudio2.h"

class SceneManager;
class GameObject;

class SceneBase : Uncopyable {
private:
	SceneManager*	mManager;

	std::vector< GameObject*> mObjectList;			// ゲームオブジェクトを全部まとめて管理できるオブジェクトリスト
	std::vector< GameObject*> mPendingObjectList;	// 待ち状態のオブジェクト

	//Quad2D		mQuadfadein;
	//Quad2D		mQuadfadeout;


	bool mInitingActors;	// アクターを初期化しているかどうか
	bool mUpdatingActors;	// アクターを更新しているかどうか

protected:
	Float3 mCameraLookat;			// カメラの注視点

public:
	SceneBase();
	void SetSceneManager(SceneManager* _sm) {
		mManager = _sm;
	}

	virtual ~SceneBase();

	// リストに追加
	void AddGameObject(GameObject* _object);
	// リストから削除
	void RemoveGameObject(GameObject* _object);

	// オブジェクト初期化
	bool Init();
	// オブジェクト更新
	void Update();
	// オブジェクト描画
	void Render();

	// 各シーンの初期化
	virtual void SceneInit() = 0;
	// 各シーンの更新
	virtual void SceneUpdate() = 0;
	// 各シーンの描画
	virtual void SceneRender() = 0;

	// imguiデバッグ
	virtual void ImguiDebug()/* = 0*/;
	// 終了
	virtual bool Dispose();

	// フェード処理

	virtual void UpdateFadeIn(double t);
	virtual void UpdateFadeOut(double t);

	virtual void DrawFadeIn(double t);
	virtual void DrawFadeOut(double t);
};