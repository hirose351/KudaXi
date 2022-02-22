#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include		<dinput.h>
#include		<wrl/client.h>
#include		"../util/uncopyable.h"

using Microsoft::WRL::ComPtr;

class CDirectInput :Uncopyable
{
private:
	ComPtr<IDirectInput8>			mpDinput;
	ComPtr<IDirectInputDevice8>		mpDiKeyboard;
	ComPtr<IDirectInputDevice8>		mpDiMouse;

	char					mKeyBuffer[256];		// �L�[�{�[�h�o�b�t�@
	char					mOldKeyBuffer[256];		// �O��̓��̓L�[�{�[�h�o�b�t�@
	DIMOUSESTATE2			mMouseState;			// �}�E�X�̏��
	DIMOUSESTATE2			mMouseStateTrigger;		// �}�E�X�̏��
	POINT					mMousePoint;			// �}�E�X���W
	int						mWidth;					// �}�E�X�̂w���W�ő�
	int						mHeight;				// �}�E�X�̂x���W�ő�
	HWND					mHwnd;
	CDirectInput() :mpDinput(nullptr), mpDiKeyboard(nullptr), mpDiMouse(nullptr) {	}

public:
	static CDirectInput& GetInstance() {
		static CDirectInput Instance;
		return Instance;
	}

	~CDirectInput() {
		Exit();
	}

	//----------------------------------
	// DirectInput ��������
	//
	//		P1 : �C���X�^���X�l
	//		P2 : �E�C���h�E�n���h���l
	//
	//	�߂�l
	//		true : ����������
	//		false : ���������s
	//----------------------------------
	bool Init(HINSTANCE hInst, HWND hwnd, int width, int height) {
		HRESULT	hr;
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&mpDinput, NULL);
		if (FAILED(hr))
			return false;

		// �L�[�{�[�h�f�o�C�X����
		mpDinput->CreateDevice(GUID_SysKeyboard, &mpDiKeyboard, NULL);
		if (FAILED(hr))
			return false;

		// �f�[�^�t�H�[�}�b�g�̐ݒ�
		hr = mpDiKeyboard->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(hr))
			return false;

		// �������x���̐ݒ�
		hr = mpDiKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
			return false;

		// �}�E�X�f�o�C�X����
		mpDinput->CreateDevice(GUID_SysMouse, &mpDiMouse, NULL);
		if (FAILED(hr))
			return false;

		// �f�[�^�t�H�[�}�b�g�̐ݒ�
		hr = mpDiMouse->SetDataFormat(&c_dfDIMouse2);
		if (FAILED(hr))
			return false;

		// �������x���̐ݒ�
		hr = mpDiMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
			return false;

		// �f�o�C�X�̐ݒ�
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;							// ���Βl���[�h
		mpDiMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);		// �����[�h�̐ݒ�


		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwObj = DIJOFS_X;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = 0;
		diprg.lMax = width - 1;

		mpDiMouse->SetProperty(DIPROP_RANGE, &diprg.diph);		// �w�����͈̔͂��w��
		diprg.diph.dwObj = DIJOFS_Y;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = 0;
		diprg.lMax = height - 1;
		mpDiMouse->SetProperty(DIPROP_RANGE, &diprg.diph);	// �x�����͈̔͂��w��

		mHwnd = hwnd;

		mHeight = height;
		mWidth = width;

		return true;
	}

	//----------------------------------
	// �}�E�X��Ԏ擾����
	//----------------------------------
	void GetMouseState() {
		HRESULT	hr;

		DIMOUSESTATE2		mouseStateOld = mMouseState;

		GetCursorPos(&mMousePoint);
		ScreenToClient(mHwnd, &mMousePoint);

		// �f�o�C�X�̔F��
		hr = mpDiMouse->Acquire();

		hr = mpDiMouse->GetDeviceState(sizeof(mMouseState), &mMouseState);
		if (SUCCEEDED(hr))
		{
			for (int cnt = 0; cnt < 8; cnt++)
			{
				mMouseStateTrigger.rgbButtons[cnt] = ((mouseStateOld.rgbButtons[cnt] ^ mMouseState.rgbButtons[cnt]) & mMouseState.rgbButtons[cnt]);
			}
		}
		else
		{
			if (hr == DIERR_INPUTLOST)
			{
				// �f�o�C�X�̔F��
				hr = mpDiMouse->Acquire();
			}
		}
	}

	//----------------------------------
	// �}�E�X�w���W�擾����
	//----------------------------------
	int GetMousePosX() const {
		return mMousePoint.x;
	}

	//----------------------------------
	// �}�E�X�x���W�擾����
	//----------------------------------
	int GetMousePosY() const {
		return mMousePoint.y;
	}

	//----------------------------------
	// �}�E�X���{�^���`�F�b�N
	//----------------------------------
	bool GetMouseLButtonCheck() const {
		if (mMouseState.rgbButtons[0] & 0x80)
			return true;
		return false;
	}

	//----------------------------------
	// �}�E�X�E�{�^���`�F�b�N
	//----------------------------------
	bool GetMouseRButtonCheck() const {
		if (mMouseState.rgbButtons[1] & 0x80)
			return true;
		return false;
	}

	//----------------------------------
	// �}�E�X�����{�^���`�F�b�N
	//----------------------------------
	bool GetMouseCButtonCheck() const {
		if (mMouseState.rgbButtons[2] & 0x80)
			return true;
		return false;
	}

	//----------------------------------
	// �}�E�X���{�^���`�F�b�N(�g���K�[)
	//----------------------------------
	bool GetMouseLButtonTrigger() const {
		if (mMouseStateTrigger.rgbButtons[0] & 0x80)
			return true;
		return false;
	}

	//----------------------------------
	// �}�E�X�E�{�^���`�F�b�N(�g���K�[)
	//----------------------------------
	bool GetMouseRButtonTrigger() const {
		if (mMouseStateTrigger.rgbButtons[1] & 0x80)
			return true;
		return false;
	}

	//----------------------------------
	// �}�E�X�����{�^���`�F�b�N(�g���K�[)
	//----------------------------------
	bool GetMouseCButtonTrigger() const {
		if (mMouseStateTrigger.rgbButtons[2] & 0x80)
			return true;
		return false;
	}

	//----------------------------------
	// �L�[�{�[�h�o�b�t�@�擾����
	//----------------------------------
	void GetKeyBuffer() {
		HRESULT	hr;
		// �f�o�C�X�̔F��
		hr = mpDiKeyboard->Acquire();
		// �O��̏�Ԃ�ۑ�
		memcpy(&mOldKeyBuffer, mKeyBuffer, sizeof(mKeyBuffer));
		hr = mpDiKeyboard->GetDeviceState(sizeof(mKeyBuffer), (LPVOID)&mKeyBuffer);
		if (hr == DIERR_INPUTLOST)
		{
			// �f�o�C�X�̔F��
			hr = mpDiKeyboard->Acquire();
		}
	}

	//----------------------------------
	// �L�[��������Ă��邩�ǂ������`�F�b�N����
	//		p1 :�@�`�F�b�N�������L�[�ԍ�
	//	�߂�l
	//		true : �w�肳�ꂽ�L�[��������Ă���
	//----------------------------------
	bool CheckKeyBuffer(int keyno) {
		if (mKeyBuffer[keyno] & 0x80)
			return true;
		return false;
	}

	//----------------------------------
	// �L�[��������Ă��邩�ǂ������`�F�b�N����
	//		p1 :�@�`�F�b�N�������L�[�ԍ�(�g���K�[)
	//	�߂�l
	//		true : �w�肳�ꂽ�L�[��������Ă���
	//----------------------------------
	bool CheckKeyBufferTrigger(int keyno) {
		if (((mKeyBuffer[keyno] ^ mOldKeyBuffer[keyno]) & mKeyBuffer[keyno]) & 0x80)
			return true;
		return false;
	}

	//----------------------------------
	// DirectInput �I������
	//----------------------------------
	void Exit() {
	}
};
