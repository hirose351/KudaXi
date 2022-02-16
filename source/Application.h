#pragma once
#include	<Windows.h>
#include	<stdio.h>
#include	<cinttypes>

class Application
{
private:
	HWND					mHwnd;							// Windowハンドル
	HINSTANCE				mHinst;							// インスタンスハンドル
	uint32_t				mSystemCounter;					// システムカウンタ
	FILE*					mpFile;							// デバッグ用コンソール

	Application();

	Application(const Application&);					// コピー
	Application& operator = (const Application&) {}		// =

	void Input(uint64_t deltataime);					// 入力
	void Update(uint64_t deltataime);					// 更新
	void Render(uint64_t deltataime);					// 描画

public:
	static const char*			WINDOW_TITLE;					// = "アプリケーションクラス";
	static const char*			WINDOW_CLASS_NAME;				// = "win32app";

	static const uint32_t		WINDOW_STYLE_WINDOWED;			// = (WS_VISIBLE | WS_CAPTION | WS_SYSMENU);
	static const uint32_t		WINDOW_EX_STYLE_WINDOWED;		// = (0);
	static const uint32_t		WINDOW_STYLE_FULL_SCREEN;		// = (WS_VISIBLE | WS_POPUP);
	static const uint32_t		WINDOW_EX_STYLE_FULL_SCREEN;	// = (0);

	static const uint32_t		CLIENT_WIDTH;					// = 1280;
	static const uint32_t		CLIENT_HEIGHT;					// = 720;

	static uint32_t				SYSTEM_WIDTH;					// = 0;
	static uint32_t				SYSTEM_HEIGHT;					// = 0;

	static const float			FPS;							// = 60;

	virtual ~Application();
	// システム幅高さ初期化
	static void InitSystemWH();

	// システム有効化
	bool Init(HINSTANCE h_cpInstance);
	// システム無効化
	void Dispose();
	unsigned long MainLoop();

	// インスタンス取得
	static Application* Instance();
	// ウィンドウハンドル取得
	HWND			 	GetHWnd();
	// インスタンスハンドル取得
	HINSTANCE			GetHInst();
};
