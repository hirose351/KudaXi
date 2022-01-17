#pragma once
#include	"../../system/util/vector.h"
#include	"../manager/scene_manager.h"
#include	"../manager/draw_manager.h"
#include	"../../system/util/dixsmartptr.h"
#include	"XAudio2.h"

class SceneManager;
class GameObject;

class SceneBase : Uncopyable {
protected:
	SceneManager*	mpSceneManager;
	DrawManager mDrawManager;

	std::list<Dix::sp<GameObject>> mpObjectList;			// ゲームオブジェクトを全部まとめて管理できるオブジェクトリスト
	std::list<Dix::sp<GameObject>> mpPendingObjectList;	// 待ち状態のオブジェクト

	bool mInitingObjects;			// オブジェクトを初期化しているかどうか
	bool mUpdatingObjects;			// オブジェクトを更新しているかどうか
	bool mIsPause = false;			// ポーズ状態か

	Float3 mCameraLookat;			// カメラの注視点
	class FadeScreen* fade;

public:
	SceneBase();
	virtual ~SceneBase();

	void SetSceneManager(SceneManager* _sm) {
		mpSceneManager = _sm;
	}

	// リストに追加
	void AddGameObject(Dix::sp<GameObject> _object);
	// リストから削除
	void RemoveGameObject(GameObject* _object);

	// オブジェクト初期化
	bool Init();
	// オブジェクト更新
	void Update();
	// オブジェクト描画
	void Render();
	// imguiデバッグ
	void ImguiDebug();
	// 終了
	bool Dispose();

	// 各シーンの変更後処理(シーンが切り換わる度に呼ばれる)
	virtual void SceneAfter() = 0;
	// 各シーンの初期化(生成時に一度だけ呼ばれる)
	virtual void SceneInit() = 0;
	// 各シーンの更新
	virtual void SceneUpdate() = 0;

	void DrawFadeIn();
	void DrawFadeOut();

	void SetIsPause(bool _flg) { mIsPause = _flg; }
	bool GetIsPause() { return mIsPause; }

	void AddDrawComponent(class DrawComponentBase* _c);
	void RemoveDrawComponent(class DrawComponentBase* _c);
};