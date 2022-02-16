#pragma once
#include	<Windows.h>
#include	<stdio.h>
#include	<cinttypes>

class Application
{
private:
	HWND					mHwnd;							// Window�n���h��
	HINSTANCE				mHinst;							// �C���X�^���X�n���h��
	uint32_t				mSystemCounter;					// �V�X�e���J�E���^
	FILE*					mpFile;							// �f�o�b�O�p�R���\�[��

	Application();

	Application(const Application&);					// �R�s�[
	Application& operator = (const Application&) {}		// =

	void Input(uint64_t deltataime);					// ����
	void Update(uint64_t deltataime);					// �X�V
	void Render(uint64_t deltataime);					// �`��

public:
	static const char*			WINDOW_TITLE;					// = "�A�v���P�[�V�����N���X";
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
	// �V�X�e��������������
	static void InitSystemWH();

	// �V�X�e���L����
	bool Init(HINSTANCE h_cpInstance);
	// �V�X�e��������
	void Dispose();
	unsigned long MainLoop();

	// �C���X�^���X�擾
	static Application* Instance();
	// �E�B���h�E�n���h���擾
	HWND			 	GetHWnd();
	// �C���X�^���X�n���h���擾
	HINSTANCE			GetHInst();
};
