#pragma once
#include	"../../system/dx11/util/dx11mathutil.h"
#include	"../gameobject/skydome.h"
#include	"../manager/SceneManager.h"
#include	"../manager/SelectManager.h"

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


public:
	MaingameScene();
	~MaingameScene();

	bool Init() override;
	void Update() override;
	void Render() override;
	void ImguiDebug() override;
	bool Dispose() override;
};