#pragma once
#include	"gameobject.h"
#include	"../state/dice_state.h"
#include	"../../system/util/CBillboard.h"

using Microsoft::WRL::ComPtr;

class Dice : public GameObject
{
private:
	float mBeforeFrameAng = 0;										// 
	float mBeforeFramePos = 0;										// 
	INT3 mMapPos;													// �}�b�v��̈ʒu
	DiceFruit mTopDiceTypeFruit;									// ��ʂ̉ʕ�
	int mTopDiceTypeNum;											// ��ʂ̐���
	DiceStatus mDiceSts = DiceStatus::eNormal;						// ���
	DiceStatus mDiceMoveSts = DiceStatus::eNormal;					// �ړ����̏��
	Direction mDirection = Direction::eNeutral;						// �T�C�R���̈ړ�����	

	DirectX::XMFLOAT4X4 mStartMtx;									// 1�t���[���ł̕ω���\���s��	
	DirectX::XMFLOAT4X4 mTargetMtx;									// 1��]�ł̕ω���\���s��
	Float3 mRotateStartPos;											// �L�[���͂��ꂽ�ۂ̊J�n�ʒu	

	const int mMoveCnt = 14;										// 90�x��]�A�������̂ɕK�v�ȍX�V��
	int mCrrentRotCnt = 0;											// ���̉�]��	

	int mCrrentPushCnt = 0;											// ���̉�]��
	const float mPushPositionPerFrame = DICE_SCALE / mMoveCnt;		// 

	const float mUpCnt = 200.0f;									// 
	const float mDownCnt = 500.0f;									// 
	const float mUpPositionPerFrame = DICE_SCALE / mUpCnt;			// 
	const float mDownPositionPerFrame = DICE_SCALE / mDownCnt;		// 

	int mChainCnt = 0;	// �`�F�C����

	// �������オ��
	void Up();
	// ��������������
	void Down();

protected:	// �ʎ�ނ�Dice�����Ƃ��͂��̊֐��̒��g��ς���
	// �ړ�
	virtual void Push();
	// ��]
	virtual void Roll();

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

	// ��̖ʂ����
	void SetOverPlane();

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
		return mDiceSts;
	}

	// ���f���̏�Ԃ��擾����(����ł��邩)
	DiceStatus GetDiceMoveStatus() {
		return mDiceMoveSts;
	}

	Direction GetDirection() {
		return mDirection;
	}

	// �n�b�s�[�����Ăяo��
	void SetHappyOne();

	void SetMapPos(INT3 _i3) { mMapPos = _i3; };
	INT3 GetMapPos() { return mMapPos; };

	bool GetPushEnd() { return mCrrentPushCnt == mMoveCnt; }
	bool GetRollEnd() { return mCrrentRotCnt >= mMoveCnt; }

	void SetDiceSts(DiceStatus _sts) { mDiceSts = _sts; }
	void SetChainCnt(int _chainCnt) { mChainCnt = _chainCnt; }
	int GetChainCnt() { return mChainCnt; }
};

