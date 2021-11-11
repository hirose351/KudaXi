//*****************************************************************************
//!	@file	WinMain.cpp
//!	@brief	
//!	@note	�A�v���P�[�V�����J�n
//!	@author	
//*****************************************************************************

#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "d3d11.lib")			// �����N�Ώۃ��C�u������d3d11.lib��ǉ�
#pragma comment(lib, "dxgi.lib")			// �����N�Ώۃ��C�u������dxgxi.lib��ǉ�
#pragma comment(lib, "D3DCompiler.lib")		// �����N�Ώۃ��C�u������d3dcompiler.lib��ǉ�
#pragma comment(lib, "winmm.lib")			// �����N�Ώۃ��C�u������winmm.lib��ǉ�
#pragma comment(lib, "directxtex.lib")		// �����N�Ώۃ��C�u������winmm.lib��ǉ�
#pragma comment(lib, "dxguid.lib")			// �����N�Ώۃ��C�u������dxguid.lib��ǉ�
#pragma comment(lib, "dinput8.lib")			// �����N�Ώۃ��C�u������dinput8.lib��ǉ�
#pragma comment(lib, "assimp-vc141-mtd.lib") // �����N�Ώۃ��C�u������assimp-vc141-mtd.lib��ǉ�

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	<Windows.h>
#include	"Application.h"

//==============================================================================
//!	@fn		WinMain
//!	@brief	�G���g���|�C���g
//!	@param	�C���X�^���X�n���h��
//!	@param	�Ӗ��Ȃ�
//!	@param	�N�����̈���������
//!	@param	�E�C���h�E�\�����[�h
//!	@retval	TRUE�@�����I��/FALSE�@���s�I��
//!	@note	
//==============================================================================
INT APIENTRY WinMain(HINSTANCE  h_hInst,
					 HINSTANCE  h_hPrevInst,
					 LPSTR		h_lpszArgs,
					 int		h_nWinMode)
{
	// ���������[�N���m
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(9554);
	//_CrtSetBreakAlloc(9553);
	//_CrtSetBreakAlloc(9552);

	// �A�v���P�[�V������������
	Application* App = Application::Instance();		// �C���X�^���X�擾
	App->Init(h_hInst);

	// �E�C���h�E��\������
	ShowWindow(App->GetHWnd(), h_nWinMode);
	UpdateWindow(App->GetHWnd());

	// ���C�����[�v
	long ret = App->MainLoop();

	// �A�v���P�[�V�����I������
	App->Dispose();

	return ret;
}


//******************************************************************************
//	End of file.
//******************************************************************************