#pragma once
#include	"gameobject.h"
#include	"../../system/model/ModelMgr.h"
#include	"../component/allcomponents.h"
#include	"../state/dice_state.h"

using Microsoft::WRL::ComPtr;

class Dice : public GameObject
{
private:
	INT3 mMapPos;										// �}�b�v��̈ʒu
	DICEFRUIT mTopDiceTypeFruit;								// ��ʂ̉ʕ�
	int mTopDiceTypeNum;								// ��ʂ̐���
	DICESTATUS mSts;									// ���
	Direction mDirection = Direction::eNeutral;			// �T�C�R���̈ړ�����	

	DirectX::XMFLOAT4X4 mMtxFrame;						// 1�t���[���ł̕ω���\���s��	
	Float3 mRotateStartPos;								// �L�[���͂��ꂽ�ۂ̊J�n�ʒu	

	const int mMoveCnt = 16;							// 90�x��]�A�������̂ɕK�v�ȍX�V��
	int mCrrentRotCnt = 0;								// ���̉�]��	
	const float mRotAnglePerFrame = 90.0f / mMoveCnt;	// 1�񓖂���̉�]�p�x	

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
	// �オ��
	void Up();
	// ��������������
	void Down();

public:
	Dice() :GameObject(("Dice"), ObjectType::eDice) {
		bool sts = ModelMgr::GetInstance().LoadModel(
			"assets/model/dice/Dice.fbx",
			"shader/vs.hlsl", "shader/pstexcol.hlsl",
			"assets/model/dice/");
		if (!sts)
		{
			MessageBox(nullptr, "Dice���f�� �ǂݍ��݃G���[", "error", MB_OK);
		}
		AddComponent<Component::ModelComponent>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/dice/Dice.fbx"));
		//AddComponent<Component::CollisionComponent>()->SetInitState(ObjectTag::DiceTop, Float3(0, 7.65f, 0), Float3(8.5f, 1.7f, 8.5f), DirectX::XMFLOAT4(0, 1, 0, 0.3f));
		//AddComponent<Component::CollisionComponent>()->SetInitState(ObjectTag::Dice, Float3(0, -0.85f, 0), Float3(8.5f, 6.8f, 8.5f), DirectX::XMFLOAT4(0, 0, 1, 0.3f));
		AddComponent<Component::CollisionComponent>()->SetInitState(ObjectTag::Dice, Float3(0, 0, 0), Float3(DICE_SCALE_HALF), DirectX::XMFLOAT4(0, 0, 1, 0.3f));
		ObjectInit();
	}
	~Dice()override;

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectDraw()override;
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

	// ��ʎ擾
	DICEFRUIT GetTopDiceType() {
		return mTopDiceTypeFruit;
	}
	// ��ʎ擾
	int GetTopDiceTypeNum() {
		return mTopDiceTypeNum;
	}

	// ���f���̏�Ԃ��擾����(����ł��邩)
	DICESTATUS GetDiceStatus() {
		return mSts;
	}

	Direction GetDirection() {
		return mDirection;
	}

	// 1�}�X���ړ�
	void MoveDiceScale(Direction _direction);

	void SetMapPos(INT3 _i3) { mMapPos = _i3; };
	INT3 GetMapPos() { return mMapPos; };

	void OnCollisionEnter(ComponentBase* _oher) override;
	void OnCollisionStay(ComponentBase* _oher) override;
	void OnCollisionExit(ComponentBase* _oher) override;

	float GetmPushPositionPerFrame() { return mPushPositionPerFrame; }
	float GetmRollmRotAnglePerFrame() { return mRotAnglePerFrame; }
	bool GetPush() { return mCrrentPushCnt == mMoveCnt; }
	bool GetRoll() { return mCrrentRotCnt >= mMoveCnt - 1; }
};

