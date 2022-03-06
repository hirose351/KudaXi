#pragma once
#pragma comment (lib, "xinput.lib")
#include		<Windows.h>
#include		<Xinput.h>
#include		<DirectXMath.h>
#include		"../util/uncopyable.h"

#define DPAD_UP          0x0001
#define DPAD_DOWN        0x0002
#define DPAD_LEFT        0x0004
#define DPAD_RIGHT       0x0008
#define DPAD_START       0x0010
#define DPAD_BACK        0x0020
#define DPAD_L_THUMB     0x0040
#define DPAD_THUMB_R     0x0080
#define DPAD_SHOULDER_L  0x0100
#define DPAD_SHOULDER_R  0x0200
#define DPAD_A           0x1000
#define DPAD_B           0x2000
#define DPAD_X           0x4000
#define DPAD_Y           0x8000

class ControllerInput :Uncopyable
{
private:
	XINPUT_STATE mState;		// コントローラーのステート
	XINPUT_STATE mBeforeState;	// 前フレームのコントローラーのステート

	// Rスティック関係
	DirectX::XMFLOAT2 mRStick;
	bool mRXFlg, mRYFlg;
	int mRXCnt = 0, mRYCnt = 0;

	// Lスティック関係
	DirectX::XMFLOAT2 mLStick;
	bool mLXFlg, mLYFlg;
	int mLXCnt = 0, mLYCnt = 0;

	// Rトリガー関係
	int mRTrigger;
	bool mRTFlg;

	// Lトリガー関係
	int mLTrigger;
	bool mLTFlg;

	// ボタン関係
	bool mButtonFlg;

public:
	static ControllerInput& GetInstance() {
		static ControllerInput Instance;
		return Instance;
	}

	//アップデートより先に回してください
	void Input();

	// コントローラーのステート全取得
	XINPUT_STATE GetControllerState();

	// Rスティック関係
	DirectX::XMFLOAT2 GetRStickValue();					// Rスティックの値取得
	bool GetRStickXDown(int i = 0, bool j = false);		// RスティックがX方向に倒れてるか (boolがtrueでトリガーにできる)
	bool GetRStickYDown(int i = 0, bool j = false);		// RスティックがY方向に倒れてるか (boolがtrueでトリガーにできる)
	bool GetRStickXButton(int i, int j, int k);			// ボタンの状態 (iが正か負、jがインターバル、kがその後の間隔)
	bool GetRStickYButton(int i, int j, int k);			// ボタンの状態 (iが正か負、jがインターバル、kがその後の間隔)

	// Lスティック関係
	DirectX::XMFLOAT2  GetLStickValue();				// Lスティックの値取得
	bool GetLStickXDown(int i = 0, bool j = false);		// LスティックがX方向に倒れてるか (boolがtrueでトリガーにできる)
	bool GetLStickYDown(int i = 0, bool j = false);		// LスティックがY方向に倒れてるか (boolがtrueでトリガーにできる)
	bool GetLStickXButton(int i, int j, int k);			// ボタンの状態 (iが正か負、jがインターバル、kがその後の間隔)
	bool GetLStickYButton(int i, int j, int k);			// ボタンの状態 (iが正か負、jがインターバル、kがその後の間隔)

	// Rトリガー関係
	int GetRTriggerValue();								// Rトリガーの値取得
	bool GetRTriggerDown(int i = 0, bool j = false);	// Rトリガーが倒れてるか (boolがtrueでトリガーにできる)

	// Lトリガー関係
	int GetLTriggerValue();								// Lトリガーの値取得
	bool GetLTriggerDown(int i = 0, bool j = false);	// Lトリガーが倒れてるか (boolがtrueでトリガーにできる)

	// ボタン関係
	bool GetButtonState(int);							// ボタンが押されたか (intに対応する定数を入れる)
	bool GetButtonStateTrigger(int);					// トリガーver（多分違う方法考えたほうがいい）

};

