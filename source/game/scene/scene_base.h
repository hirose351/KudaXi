#pragma once
#include	"../../system/util/vector.h"
#include	"../manager/scene_manager.h"
#include	"../manager/draw_manager.h"
#include	"XAudio2.h"

class SceneManager;
class GameObject;

class SceneBase : Uncopyable {
protected:
	SceneManager*	mSceneManager;

	std::vector< GameObject*> mObjectList;			// ゲームオブジェクトを全部まとめて管理できるオブジェクトリスト
	std::vector< GameObject*> mPendingObjectList;	// 待ち状態のオブジェクト

	DrawManager mDrawManager;

	bool mInitingActors;	// アクターを初期化しているかどうか
	bool mUpdatingActors;	// アクターを更新しているかどうか

	bool mIsPause = false;			// ポーズ状態か

	Float3 mCameraLookat;			// カメラの注視点

public:
	SceneBase();
	void SetSceneManager(SceneManager* _sm) {
		mSceneManager = _sm;
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


	// 各シーンの変更後処理(シーンが切り換わる度に呼ばれる)
	virtual void SceneAfter() = 0;
	// 各シーンの初期化(生成時に一度だけ呼ばれる)
	virtual void SceneInit() = 0;
	// 各シーンの更新
	virtual void SceneUpdate() = 0;

	// imguiデバッグ
	virtual void ImguiDebug()/* = 0*/;
	// 終了
	virtual bool Dispose();

	void SetIsPause(bool _flg) { mIsPause = _flg; }
	bool GetIsPause() { return mIsPause; }

	void AddDrawComponent(class DrawComponentBase* _c);
	void RemoveDrawComponent(class DrawComponentBase* _c);
};