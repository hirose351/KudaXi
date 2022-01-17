#pragma once
#include	"../../system/util/dixsmartptr.h"
#include	"../component/player_controller.h"
#include	"../gameobject/transform.h"
#include	"../manager/dice_manager.h"
#include	"state_base.h"

enum
{
	eMove,
	ePush,
	eRoll,
};

namespace Component
{
	class PlayerController;
}

class DIce;

class PlayerStateBase :public StateBase
{
protected:
	//Dix::wp<Component::PlayerController> mHolder;			// ������
	Component::PlayerController* mHolder;			// ������
	Dix::wp<Transform>			 mTransform;		// �ʒu��]�傫��
	Dix::wp<Direction>			 mDirection;		// �ړ�����
	Dice*						 mpOperationDice;	// ���삷��Dice
	Foot*						 mFoot;				// ����
	INT3						 mMapPos;			// �}�b�v��̈ʒu

	/// Todo:�A�j���[�V�����Ǘ��������Ő錾
public:
	virtual ~PlayerStateBase() {};
	// ���삷��ϐ��o�^
	virtual void Start(Component::PlayerController* _c);

	void SetOperationDice(Dice* _dice) { mpOperationDice = _dice; }
	Dice* GetOperationDice() { return mpOperationDice; }

	INT3 GetMapPos() { return mMapPos; }
	void SetMapPos(INT3 _pos) { mMapPos = _pos; }
};