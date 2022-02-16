#define _CRTDBG_MAP_ALLOC
#include	<Windows.h>
#include	<chrono>
#include	<thread>
#include	"application.h"
#include	"system/windows/CWindow.h"
#include	"macro.h"
#include	"game.h"
#include	"game/manager/scene_manager.h"

const char*			Application::WINDOW_TITLE = "����Xi";
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
	// �������[���[�N���o�ݒ�
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �R���\�[�������蓖�Ă�
	AllocConsole();

	// �W���o�͂̊��蓖��
	freopen_s(&mpFile, "CON", "w", stdout);

	// ���ƍ���������
	InitSystemWH();

	// �E�C���h�E�쐬
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
	// �W���o�̓N���[�Y
	fclose(mpFile);
	// �R���\�[���J��
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

	// �Q�[���̏�������
	GameInit();

	// �^�C�}�𑜓x���~���b��
	::timeBeginPeriod(1);

	while (window->ExecMessage() && !SceneManager::GetInstance()->GetGameEndFlg())
	{

		// timeGetTime�֐��́A�~���b�P�ʂŃV�X�e���������擾���܂��B 
		// �V�X�e�����Ԃ́AWindows���N�����Ă���̌o�ߎ��Ԃł��B
		current_time = ::timeGetTime();

		uint64_t delta_time = current_time - last_time;

		last_time = current_time;

		GameInput(delta_time);		// �Q�[������	
		GameUpdate(delta_time);		// �Q�[���X�V
		GameRender(delta_time);		// �Q�[���`��

		int64_t sleep_time = 16666 - delta_time;

		if (sleep_time > 0)
		{
			float tt = sleep_time / 1000.0f;
			//std::cout << "sleep:" << tt << "\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(tt)));
			// �w�肵�����Ύ��Ԃ������X���b�h���u���b�N���� (function template)
		}
	}

	// �^�C�}�𑜓x�����ɖ߂�
	::timeEndPeriod(1);

	// �Q�[���̏I������
	GameDispose();

	// ���������[�N���o
	//_CrtDumpMemoryLeaks();

	return window->GetMessage();
}

void Application::Input(uint64_t deltataime)
{
}

void Application::Update(uint64_t deltataime)
{
	// �V�X�e���J�E���^
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
