#pragma once
#include	"gameobject.h"
#include	"transform.h"
#include	"gameobject_utility.h"
#include	"../../system/model/CModel.h"

class Player :public GameObject, Transform
{
private:
	CModel*	mpModel;							// �R�c���f��
	Transform mTramsform;						// �ʒu�A�p���A�傫��
	bool mIsDiceMove = false;					// �T�C�R������]���Ă��邩
	DIRECTION mDiceMoveDirection;				// �T�C�R������]���������
	DIRECTION mDirection;						// �v���C���[�̕���(�L�[�Q��)

	///////////////////////////////////////////////////////////////

	Vector3					mMove;					// �ړ���
	Vector3					mDestrot;				// �ڕW�p��
	DIRECTION				mMoveKeySts;			// ������Ă���ړ��L�[


	// ���f���|�C���^�擾
	void SetModel(CModel* p) {
		mpModel = p;
	}
public:
	Player();
	~Player();

	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize()override;
};