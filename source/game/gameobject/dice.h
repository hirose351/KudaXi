#pragma once
#include	"gameobject.h"
#include	"../../system/model/ModelMgr.h"
#include	"../component/allcomponents.h"
#include	"../state/dice_state.h"
#include	"../../system/util/CBillboard.h"
#include	"../../system/util/StopWatch.h"

using Microsoft::WRL::ComPtr;

class Dice : public GameObject
{
private:
	float mBeforeFrameAng = 0;							// 
	float mBeforeFramePos = 0;							// 
	//float mThunderAlha = 1.0f;							// �����l
	//Billboard mThunder;								// ��������
	INT3 mMapPos;										// �}�b�v��̈ʒu
	DiceFruit mTopDiceTypeFruit;						// ��ʂ̉ʕ�
	int mTopDiceTypeNum;								// ��ʂ̐���
	DiceStatus mSts;									// ���
	Direction mDirection = Direction::eNeutral;			// �T�C�R���̈ړ�����	

	DirectX::XMFLOAT4X4 mStartMtx;						// 1�t���[���ł̕ω���\���s��	
	DirectX::XMFLOAT4X4 mTargetMtx;						// 1��]�ł̕ω���\���s��
	Float3 mRotateStartPos;								// �L�[���͂��ꂽ�ۂ̊J�n�ʒu	

	const int mMoveCnt = 16;							// 90�x��]�A�������̂ɕK�v�ȍX�V��
	int mCrrentRotCnt = 0;								// ���̉�]��	

	int mCrrentPushCnt = 0;								// ���̉�]��
	const float mPushPositionPerFrame = DICE_SCALE / mMoveCnt;

	const int mUpCnt = 150;
	const float mUpPositionPerFrame = DICE_SCALE / mUpCnt;

	// ��̖ʂ����
	void SetOverPlane();
	// ��]�ړ��Z�b�g
	void SetRollDirection(Direction _direction);

	// �ړ�����
	void Push();
	// ��]����
	void Roll();
	// �������オ��
	void Up();
	// ��������������
	void Down();

public:
	Dice();
	virtual ~Dice();

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectImguiDraw()override {};
	void Uninit()override;

	// �����ʃZ�b�g
	void SetInitType() {
		SetOverPlane();
	}

	void SetStartUpPosition();
	void SetDownPosition();

	// �w������Ɉړ�
	bool SetPushAction(Direction _direction);
	// �w������ɉ�]
	bool SetRollAction(Direction _direction);
	// �w������ɃT�C�R�������邩
	bool CheckDiceDirection(Direction _direction);

	// ��ʎ擾
	DiceFruit GetTopDiceType() {
		return mTopDiceTypeFruit;
	}
	// ��ʎ擾
	int GetTopDiceTypeNum() {
		return mTopDiceTypeNum;
	}

	// ���f���̏�Ԃ��擾����(����ł��邩)
	DiceStatus GetDiceStatus() {
		return mSts;
	}

	Direction GetDirection() {
		return mDirection;
	}

	// �n�b�s�[�����Ăяo��
	void SetHappyOne();

	// 1�}�X���ړ�
	void MoveDiceScale(Direction _direction);

	void SetMapPos(INT3 _i3) { mMapPos = _i3; };
	INT3 GetMapPos() { return mMapPos; };

	void OnCollisionEnter(ComponentBase* _oher) override;
	void OnCollisionStay(ComponentBase* _oher) override;
	void OnCollisionExit(ComponentBase* _oher) override;
	bool GetPushEnd() { return mCrrentPushCnt == mMoveCnt; }
	bool GetRollEnd() { return mCrrentRotCnt >= mMoveCnt; }
};

