#pragma once
#include	"../gameobject/transform.h"
#include	"../../system/util/dixsmartptr.h"


enum Pstate {
	eStop,
	eMove,
	ePush,
	eRoll,
};

class State
{
	Dix::wp<Transform> mTramsform;

	//DIRECTION mDiceMoveDirection;				// �T�C�R������]���������
	//bool mIsDiceMove = false;					// �T�C�R������]���Ă��邩
	//DIRECTION mDirection;						// �v���C���[�̕���(�L�[�Q��)
	//Float3					mDestrot;			// �ڕW�p��
	//DIRECTION				mMoveKeySts;		// ������Ă���ړ��L�[

};