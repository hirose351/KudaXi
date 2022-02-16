#define _CRTDBG_MAP_ALLOC
#include	<Windows.h>
#include	<chrono>
#include	<thread>
#include	"application.h"
#include	"system/windows/CWindow.h"
#include	"macro.h"
#include	"game.h"
#include	"game/manager/scene_manager.h"

const char*			Application::WINDOW_TITLE = "くだXi";
const char*			Application::WINDOW_CLASS_NAME = "win32app";

const uint32_t		Application::WINDOW_STYLE_WINDOWED = (WS_VISIBLE | WS_CAPTION | WS_SYSMENU);
const uint32_t		Application::WINDOW_EX_STYLE_WINDOWED = (0);
const uint32_t		Application::WINDOW_STYLE_FULL_SCREEN = (WS_VISIBLE | WS_POPUP);
const uint32_t		Application::WINDOW_EX_STYLE_FULL_SCREEN = (0);

const uint32_t		Application::CLIENT_WIDTH = 1280;
const uint32_t		Application::CLIENT_HEIGHT = 720;

uint32_t			Application::SYSTEM_WIDTH = 0;
uint32_t			Application::SYSTEM_HEIGHT = 0;

const float			Application::FPS = 60;

Application::Application() : mSystemCounter(0)
{
}

Application :: ~Application()
{
	Dispose();
}

Application* Application::Instance()
{
	static Application Instance;

	return &Instance;
}

void Application::InitSystemWH()
{
}

bool Application::Init(HINSTANCE h_Instance)
{
	// メモリーリーク検出設定
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// コンソールを割り当てる
	AllocConsole();

	// 標準出力の割り当て
	freopen_s(&mpFile, "CON", "w", stdout);

	// 幅と高さ初期化
	InitSystemWH();

	// ウインドウ作成
	CWindow* window = CWindow::Instance();
	window->RegisterClass(h_Instance, WINDOW_CLASS_NAME, CS_OWNDC);

	window->SetWindow(h_Instance,
					  WINDOW_STYLE_WINDOWED,
					  WINDOW_EX_STYLE_WINDOWED,
					  NULL,
					  CLIENT_WIDTH + SYSTEM_WIDTH,
					  CLIENT_HEIGHT + SYSTEM_HEIGHT,
					  WINDOW_CLASS_NAME,
					  WINDOW_TITLE,
					  false);

	// HWND
	mHwnd = window->GetHandle();

	//
	mHinst = h_Instance;
	return true;
}

void Application::Dispose()
{
	// 標準出力クローズ
	fclose(mpFile);
	// コンソール開放
	::FreeConsole();
	return;
}

unsigned long Application::MainLoop()
{
	MSG		msg;
	ZeroMemory(&msg, sizeof(msg));

	CWindow* window = CWindow::Instance();

	uint64_t current_time = 0;
	uint64_t last_time = 0;

	// ゲームの初期処理
	GameInit();

	// タイマ解像度をミリ秒に
	::timeBeginPeriod(1);

	while (window->ExecMessage() && !SceneManager::GetInstance()->GetGameEndFlg())
	{

		// timeGetTime関数は、ミリ秒単位でシステム時刻を取得します。 
		// システム時間は、Windowsを起動してからの経過時間です。
		current_time = ::timeGetTime();

		uint64_t delta_time = current_time - last_time;

		last_time = current_time;

		GameInput(delta_time);		// ゲーム入力	
		GameUpdate(delta_time);		// ゲーム更新
		GameRender(delta_time);		// ゲーム描画

		int64_t sleep_time = 16666 - delta_time;

		if (sleep_time > 0)
		{
			float tt = sleep_time / 1000.0f;
			//std::cout << "sleep:" << tt << "\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(tt)));
			// 指定した相対時間だけ現スレッドをブロックする (function template)
		}
	}

	// タイマ解像度を元に戻す
	::timeEndPeriod(1);

	// ゲームの終了処理
	GameDispose();

	// メモリリーク検出
	//_CrtDumpMemoryLeaks();

	return window->GetMessage();
}

void Application::Input(uint64_t deltataime)
{
}

void Application::Update(uint64_t deltataime)
{
	// システムカウンタ
	mSystemCounter++;
}

void Application::Render(uint64_t deltatime)
{
}

HWND Application::GetHWnd()
{
	return mHwnd;
}

HINSTANCE Application::GetHInst()
{
	return mHinst;
}
