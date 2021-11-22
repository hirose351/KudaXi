#pragma once
#include	"IScene.h"
#include	"../../system/dx11/dx11mathutil.h"
#include	"../../system/util/dixsmartptr.h"
#include	<list>

using Microsoft::WRL::ComPtr;

// 状態
enum class GAMESTATE
{
	SETUP,	// 準備
	PLAY,	// プレイ中
	PAUSE,	// 中断
	OVER,	// ゲームオーバー
	CLEAR,	// クリア
	SELECT,	// セレクト
};

class MaingameScene : public IScene
{
private:
	std::list<Dix::sp<class GameObject>> mObjList;

public:
	MaingameScene();
	~MaingameScene();

	void AddGameObject() override;
	bool Init() override;
	void Update() override;
	void Render() override;
	void ImguiDebug() override;
	bool Dispose() override;
};