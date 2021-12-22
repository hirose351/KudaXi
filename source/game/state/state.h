#pragma once
#include	"../../system/util/dixsmartptr.h"
#include	"../component/player_controller.h"
#include	"../gameobject/transform.h"
#include	"../manager/dice_manager.h"

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

class State
{
protected:
	//Dix::wp<Component::PlayerController> mHolder;			// ������
	Component::PlayerController* mHolder;			// ������
	Dix::wp<Transform>				 mTransform;			// �ʒu��]�傫��
	Dix::wp<Direction>				 mDirection;			// �ړ�����
	Dice*				 mpOperationDice;					// �ړ�����
	Foot*				 mFoot;			// �ړ�����
	INT3 mMapPos;										// �}�b�v��̈ʒu

	/// Todo:�A�j���[�V�����Ǘ��������Ő錾
public:
	virtual ~State() {};
	// ���삷��ϐ��o�^
	virtual void Start(Component::PlayerController* _c);
	// ��������
	virtual void Init() {};
	// ���s
	virtual void Exec() = 0;
	// State�ύX�O����
	virtual void BeforeChange() {}
	// State�ύX�㏈��
	virtual void AfterChange() {}
	// 
	void SetOperationDice(Dice* _dice) { mpOperationDice = _dice; }
	Dice* GetOperationDice() { return mpOperationDice; }
};