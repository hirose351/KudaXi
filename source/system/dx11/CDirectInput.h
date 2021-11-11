#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include	<dinput.h>
#include	<wrl/client.h>

using Microsoft::WRL::ComPtr;

class CDirectInput {
private:
	ComPtr<IDirectInput8>	mDinput;
	ComPtr<IDirectInputDevice8>	mDiKeyboard;
	ComPtr<IDirectInputDevice8>	mDiMouse;

	char					mKeyBuffer[256];		// キーボードバッファ
	char					mOldKeyBuffer[256];	// 前回の入力キーボードバッファ
	DIMOUSESTATE2			mMouseState;			// マウスの状態
	DIMOUSESTATE2			mMouseStateTrigger;	// マウスの状態
	POINT					mMousePoint;			// マウス座標
	int						mWidth;			// マウスのＸ座標最大
	int						mHeight;			// マウスのＹ座標最大
	HWND					mHwnd;
	CDirectInput() :mDinput(nullptr), mDiKeyboard(nullptr), mDiMouse(nullptr) {
	}
public:

	CDirectInput(const CDirectInput&) = delete;
	CDirectInput& operator=(const CDirectInput&) = delete;
	CDirectInput(CDirectInput&&) = delete;
	CDirectInput& operator=(CDirectInput&&) = delete;

	static CDirectInput& GetInstance() {
		static CDirectInput Instance;
		return Instance;
	}

	~CDirectInput() {
		Exit();
	}

	//----------------------------------
	// DirectInput 初期処理
	//
	//		P1 : インスタンス値
	//		P2 : ウインドウハンドル値
	//
	//	戻り値
	//		true : 初期化成功
	//		false : 初期化失敗
	//----------------------------------
	bool Init(HINSTANCE hInst, HWND hwnd, int width, int height) {
		HRESULT	hr;
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&mDinput, NULL);
		if (FAILED(hr))
		{
			return false;
		}

		// キーボードデバイス生成
		mDinput->CreateDevice(GUID_SysKeyboard, &mDiKeyboard, NULL);
		if (FAILED(hr))
		{
			return false;
		}

		// データフォーマットの設定
		hr = mDiKeyboard->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(hr))
		{
			return false;
		}

		// 協調レベルの設定
		hr = mDiKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
		{
			return false;
		}

		// マウスデバイス生成
		mDinput->CreateDevice(GUID_SysMouse, &mDiMouse, NULL);
		if (FAILED(hr))
		{
			return false;
		}

		// データフォーマットの設定
		hr = mDiMouse->SetDataFormat(&c_dfDIMouse2);
		if (FAILED(hr))
		{
			return false;
		}

		// 協調レベルの設定
		hr = mDiMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
		{
			return false;
		}

		// デバイスの設定
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;							// 相対値モード
		mDiMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);		// 軸モードの設定


		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwObj = DIJOFS_X;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = 0;
		diprg.lMax = width - 1;

		mDiMouse->SetProperty(DIPROP_RANGE, &diprg.diph);		// Ｘ方向の範囲を指定
		diprg.diph.dwObj = DIJOFS_Y;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = 0;
		diprg.lMax = height - 1;
		mDiMouse->SetProperty(DIPROP_RANGE, &diprg.diph);	// Ｙ方向の範囲を指定

		mHwnd = hwnd;

		mHeight = height;
		mWidth = width;

		return true;
	}

	//----------------------------------
	// マウス状態取得処理
	//----------------------------------
	void GetMouseState() {
		HRESULT	hr;

		DIMOUSESTATE2		mouseStateOld = mMouseState;

		GetCursorPos(&mMousePoint);
		ScreenToClient(mHwnd, &mMousePoint);

		// デバイスの認識
		hr = mDiMouse->Acquire();

		hr = mDiMouse->GetDeviceState(sizeof(mMouseState), &mMouseState);
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
				// デバイスの認識
				hr = mDiMouse->Acquire();
			}
		}
	}

	//----------------------------------
	// マウスＸ座標取得処理
	//----------------------------------
	int GetMousePosX() const {
		return mMousePoint.x;
	}

	//----------------------------------
	// マウスＹ座標取得処理
	//----------------------------------
	int GetMousePosY() const {
		return mMousePoint.y;
	}

	//----------------------------------
	// マウス左ボタンチェック
	//----------------------------------
	bool GetMouseLButtonCheck() const {
		if (mMouseState.rgbButtons[0] & 0x80)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//----------------------------------
	// マウス右ボタンチェック
	//----------------------------------
	bool GetMouseRButtonCheck() const {
		if (mMouseState.rgbButtons[1] & 0x80)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//----------------------------------
	// マウス中央ボタンチェック
	//----------------------------------
	bool GetMouseCButtonCheck() const {
		if (mMouseState.rgbButtons[2] & 0x80)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//----------------------------------
	// マウス左ボタンチェック(トリガー)
	//----------------------------------
	bool GetMouseLButtonTrigger() const {
		if (mMouseStateTrigger.rgbButtons[0] & 0x80)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//----------------------------------
	// マウス右ボタンチェック(トリガー)
	//----------------------------------
	bool GetMouseRButtonTrigger() const {
		if (mMouseStateTrigger.rgbButtons[1] & 0x80)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//----------------------------------
	// マウス中央ボタンチェック(トリガー)
	//----------------------------------
	bool GetMouseCButtonTrigger() const {
		if (mMouseStateTrigger.rgbButtons[2] & 0x80)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//----------------------------------
	// キーボードバッファ取得処理
	//----------------------------------
	void GetKeyBuffer() {
		HRESULT	hr;
		// デバイスの認識
		hr = mDiKeyboard->Acquire();
		// 前回の状態を保存
		memcpy(&mOldKeyBuffer, mKeyBuffer, sizeof(mKeyBuffer));
		hr = mDiKeyboard->GetDeviceState(sizeof(mKeyBuffer), (LPVOID)&mKeyBuffer);
		if (hr == DIERR_INPUTLOST)
		{
			// デバイスの認識
			hr = mDiKeyboard->Acquire();
		}
	}

	//----------------------------------
	// キーが押されているかどうかをチェックする
	//		p1 :　チェックしたいキー番号
	//	戻り値
	//		true : 指定されたキーが押されている
	//----------------------------------
	bool CheckKeyBuffer(int keyno) {
		if (mKeyBuffer[keyno] & 0x80)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//----------------------------------
	// キーが押されているかどうかをチェックする
	//		p1 :　チェックしたいキー番号(トリガー)
	//	戻り値
	//		true : 指定されたキーが押されている
	//----------------------------------
	bool CheckKeyBufferTrigger(int keyno) {
		if (((mKeyBuffer[keyno] ^ mOldKeyBuffer[keyno]) & mKeyBuffer[keyno]) & 0x80)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//----------------------------------
	// DirectInput 終了処理
	//----------------------------------
	void Exit() {
	}
};
