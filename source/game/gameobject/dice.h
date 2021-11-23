#pragma once
#include	"gameobject.h"
#include	"transform.h"
#include	"../../system/model/CModel.h"
#include	<vector>

using Microsoft::WRL::ComPtr;

// �T�C�R���̖ʂ̎��
enum class DICETYPE {
	PEACH,
	MELON,
	BLUEBERRY,
	BANANA,
	APPLE,
	ORANGE,
	DICETYPE_MAX,
};

// �T�C�R���̏��
enum class DICESTATUS {
	NORMAL,		// �ʏ�
	DOWN,		// ����
	HALFDOWN,	// ��������
};

class Dice : public GameObject, Transform
{
private:
	CModel* mpModel;
	DICETYPE mTopDiceType;								// ���
	DICESTATUS mSts;	// �������
	DIRECTION mDirection = DIRECTION::NEUTRAL;			// �T�C�R���̈ړ�����	

	DirectX::XMFLOAT4X4 mMtxFrame;						// 1�t���[���ł̕ω���\���s��	
	Vector3 mStartPos;									// �L�[���͂��ꂽ�ۂ̊J�n�ʒu	

	int mCrrentRotCnt = 0;								// ���̉�]��
	const int mRotCnt = 12;								// 90�x��]����̂ɕK�v�ȍX�V��	
	const float mRotAnglePerFrame = 90.0f / mRotCnt;	// 1�񓖂���̉�]�p�x	

	// ��̖ʂ����
	DICETYPE OverPlane();
	// ��]�ړ��Z�b�g
	void SetRollDirection(DIRECTION _direction);

	// ���f���|�C���^�擾
	void SetModel(CModel* p) {
		mpModel = p;
	}
public:
	Dice();
	~Dice()override;

	void Init() override;
	void Update()override;
	void Draw()override;
	void Finalize()override;

	// �����ʃZ�b�g
	void SetInitType() {
		mTopDiceType = OverPlane();
	}

	// �w������Ɉړ�
	bool Push(DIRECTION _direction);
	// �w������ɉ�]
	bool Roll(DIRECTION _direction);

	// ��ʎ擾
	DICETYPE GetTopDiceType() {
		return mTopDiceType;
	}

	// ���f���̏�Ԃ��擾����(����ł��邩)
	DICESTATUS GetDiceStatus() {
		return mSts;
	}

	DIRECTION GetDirection() {
		return mDirection;
	}

	// 1�}�X���ړ�
	void MoveDiceScale(DIRECTION _direction);

};
