#pragma once
#include	"gameobject.h"
#include	"dice.h"
#include	"../../system/model/ModelMgr.h"
#include	"../component/allcomponents.h"
#include	"../state/state.h"
#include	"../manager/stagedata_manager.h"

using Microsoft::WRL::ComPtr;

class Player :public GameObject
{
private:
	Direction mDiceMoveDirection;				// �T�C�R������]���������
	Direction mDirection;						// �v���C���[�̕���(�L�[�Q��)
	Float3					mDestrot;			// �ڕW�p��
	Direction				mMoveKeySts;		// ������Ă���ړ��L�[
	Dice*	mpOperationDice;						// ���쒆�̃T�C�R��
	INT3 mMapPos;								// �}�b�v��̈ʒu
	Pstate mPstate;
	StageData stageData;

	void Move();
	void Roll();
	void Push();
	// ��]�ł��邩�m�F
	void CheckRoll();
	// �����邩�m�F
	void CheckPush();
	// �X�e�[�W�O�ɏo�Ȃ��悤����
	void StageHitCorrection();
	// �ł��߂��T�C�R����o�^
	bool SetNearestDice();
public:
	Player() : GameObject(("Player"), ObjectType::ePlayer) {
		bool sts = ModelMgr::GetInstance().LoadModel(
			"assets/model/player/player.fbx",
			"shader/vs.hlsl", "shader/ps.hlsl",
			"assets/model/player/");
		if (!sts)
		{
			MessageBox(nullptr, "Player���f�� �ǂݍ��݃G���[", "error", MB_OK);
		}
		mTransform->scale = (Float3(4, 6, 4));
		AddComponent<Component::ModelComponent>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/player/player.fbx"));
		AddComponent<Component::CollisionComponent>()->SetLocalScale(mTransform->scale);
		GetComponent<Component::CollisionComponent>()->SetColor(DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 0.01f));
		ObjectInit();
	};
	~Player();

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectDraw()override {};
	void ObjectImguiDraw()override;
	void Uninit() override;

	void SetMapPos(INT3 _i3) { mMapPos = _i3; };
	INT3 GetMapPos() { return mMapPos; };

	void OnCollisionEnter(ComponentBase* _oher) override;
	void OnCollisionStay(ComponentBase* _oher) override;
	void OnCollisionExit(ComponentBase* _oher) override;

	/// Todo:�_�u���f�B�X�p�b�`�̂����ԈႦ�Ă�̂Œ���
	void OnColEnterObj(Dice* _other);
	void OnColStayObj(Dice* _other);
	void OnColExitObj(Dice* _other);
};