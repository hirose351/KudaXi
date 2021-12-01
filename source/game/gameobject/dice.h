#pragma once
#include	"gameobject.h"
#include	"../../system/model/ModelMgr.h"
#include	"../component/allcomponents.h"

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

class Dice : public GameObject/*, CollisionInterface*/
{
private:
	//CModel* mpModel;
	DICETYPE mTopDiceType;								// ���
	DICESTATUS mSts;	// �������
	DIRECTION mDirection = DIRECTION::NEUTRAL;			// �T�C�R���̈ړ�����	

	DirectX::XMFLOAT4X4 mMtxFrame;						// 1�t���[���ł̕ω���\���s��	
	Float3 mStartPos;									// �L�[���͂��ꂽ�ۂ̊J�n�ʒu	

	int mCrrentRotCnt = 0;								// ���̉�]��
	const int mRotCnt = 12;								// 90�x��]����̂ɕK�v�ȍX�V��	
	const float mRotAnglePerFrame = 90.0f / mRotCnt;	// 1�񓖂���̉�]�p�x	

	// ��̖ʂ����
	DICETYPE OverPlane();
	// ��]�ړ��Z�b�g
	void SetRollDirection(DIRECTION _direction);

	//// ���f���|�C���^�擾
	//void SetModel(CModel* p) {
	//	mpModel = p;
	//}
public:
	Dice() :GameObject(("Dice"), ObjectType::Dice) {
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

	//void OnCollisionEnter(GameObject* _oher) override;
	//void OnCollisionStay(GameObject* _oher) override;
	//void OnCollisionExit(GameObject* _oher) override;
};

