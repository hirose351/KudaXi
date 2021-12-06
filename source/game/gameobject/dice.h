#pragma once
#include	"gameobject.h"
#include	"../../system/model/ModelMgr.h"
#include	"../component/allcomponents.h"
#include	"../../system/util/INT3.h"

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
	ROLL,		// ��]
	PUSH,		// ����
	DOWN,		// ����
	HALFDOWN,	// ��������
	UP,	// ��������
};

class Dice : public GameObject
{
private:
	INT3 mMapPos;
	DICETYPE mTopDiceType;								// ���
	DICESTATUS mSts;	// �������
	Direction mDirection = Direction::eNeutral;			// �T�C�R���̈ړ�����	

	DirectX::XMFLOAT4X4 mMtxFrame;						// 1�t���[���ł̕ω���\���s��	
	Float3 mRotateStartPos;									// �L�[���͂��ꂽ�ۂ̊J�n�ʒu	

	const int mMoveCnt = 12;							// 90�x��]�A�������̂ɕK�v�ȍX�V��
	int mCrrentRotCnt = 0;								// ���̉�]��	
	const float mRotAnglePerFrame = 90.0f / mMoveCnt;	// 1�񓖂���̉�]�p�x	

	int mCrrentPushCnt = 0;								// ���̉�]��
	const float mPushPisitionPerFrame = DICE_SCALE / mMoveCnt;

	const int mUpCnt = 150;
	const float mUpPisitionPerFrame = DICE_SCALE / mUpCnt;

	// ��̖ʂ����
	DICETYPE OverPlane();
	// ��]�ړ��Z�b�g
	void SetRollDirection(Direction _direction);

	void Up();
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
		//mTransform.SetScale(15.0f);
		AddComponent<Component::ModelComponent>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/dice/Dice.fbx"));
		AddComponent<Component::CollisionComponent>()->SetInitState(ObjectTag::DiceTop, Float3(0, 7, 0), Float3(8.5f, 8.5f*0.2f, 8.5f), DirectX::XMFLOAT4(0, 1, 0, 0.3f));
		AddComponent<Component::CollisionComponent>()->SetInitState(ObjectTag::Dice, Float3(0, -2, 0), Float3(8.5f, 8.5f*0.8f, 8.5f), DirectX::XMFLOAT4(0, 0, 1, 0.3f));
		ObjectInit();
	}
	~Dice()override;

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectDraw()override;
	void Uninit()override;

	// �����ʃZ�b�g
	void SetInitType() {
		mTopDiceType = OverPlane();
	}

	// �w������Ɉړ�
	bool Push(Direction _direction);
	// �w������ɉ�]
	bool Roll(Direction _direction);

	// ��ʎ擾
	DICETYPE GetTopDiceType() {
		return mTopDiceType;
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

	float GetmPushPositionPerFrame() { return mPushPisitionPerFrame; }
	bool GetPush() { return mCrrentPushCnt == mMoveCnt; }
};

