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
	XINPUT_STATE mState;		// �R���g���[���[�̃X�e�[�g
	XINPUT_STATE mBeforeState;	// �O�t���[���̃R���g���[���[�̃X�e�[�g

	// R�X�e�B�b�N�֌W
	DirectX::XMFLOAT2 mRStick;
	bool mRXFlg, mRYFlg;
	int mRXCnt = 0, mRYCnt = 0;

	// L�X�e�B�b�N�֌W
	DirectX::XMFLOAT2 mLStick;
	bool mLXFlg, mLYFlg;
	int mLXCnt = 0, mLYCnt = 0;

	// R�g���K�[�֌W
	int mRTrigger;
	bool mRTFlg;

	// L�g���K�[�֌W
	int mLTrigger;
	bool mLTFlg;

	// �{�^���֌W
	bool mButtonFlg;

public:
	static ControllerInput& GetInstance() {
		static ControllerInput Instance;
		return Instance;
	}

	//�A�b�v�f�[�g����ɉ񂵂Ă�������
	void Input();

	// �R���g���[���[�̃X�e�[�g�S�擾
	XINPUT_STATE GetControllerState();

	// R�X�e�B�b�N�֌W
	DirectX::XMFLOAT2 GetRStickValue();					// R�X�e�B�b�N�̒l�擾
	bool GetRStickXDown(int i = 0, bool j = false);		// R�X�e�B�b�N��X�����ɓ|��Ă邩 (bool��true�Ńg���K�[�ɂł���)
	bool GetRStickYDown(int i = 0, bool j = false);		// R�X�e�B�b�N��Y�����ɓ|��Ă邩 (bool��true�Ńg���K�[�ɂł���)
	bool GetRStickXButton(int i, int j, int k);			// �{�^���̏�� (i���������Aj���C���^�[�o���Ak�����̌�̊Ԋu)
	bool GetRStickYButton(int i, int j, int k);			// �{�^���̏�� (i���������Aj���C���^�[�o���Ak�����̌�̊Ԋu)

	// L�X�e�B�b�N�֌W
	DirectX::XMFLOAT2  GetLStickValue();				// L�X�e�B�b�N�̒l�擾
	bool GetLStickXDown(int i = 0, bool j = false);		// L�X�e�B�b�N��X�����ɓ|��Ă邩 (bool��true�Ńg���K�[�ɂł���)
	bool GetLStickYDown(int i = 0, bool j = false);		// L�X�e�B�b�N��Y�����ɓ|��Ă邩 (bool��true�Ńg���K�[�ɂł���)
	bool GetLStickXButton(int i, int j, int k);			// �{�^���̏�� (i���������Aj���C���^�[�o���Ak�����̌�̊Ԋu)
	bool GetLStickYButton(int i, int j, int k);			// �{�^���̏�� (i���������Aj���C���^�[�o���Ak�����̌�̊Ԋu)

	// R�g���K�[�֌W
	int GetRTriggerValue();								// R�g���K�[�̒l�擾
	bool GetRTriggerDown(int i = 0, bool j = false);	// R�g���K�[���|��Ă邩 (bool��true�Ńg���K�[�ɂł���)

	// L�g���K�[�֌W
	int GetLTriggerValue();								// L�g���K�[�̒l�擾
	bool GetLTriggerDown(int i = 0, bool j = false);	// L�g���K�[���|��Ă邩 (bool��true�Ńg���K�[�ɂł���)

	// �{�^���֌W
	bool GetButtonState(int);							// �{�^���������ꂽ�� (int�ɑΉ�����萔������)
	bool GetButtonStateTrigger(int);					// �g���K�[ver�i�����Ⴄ���@�l�����ق��������j

};

