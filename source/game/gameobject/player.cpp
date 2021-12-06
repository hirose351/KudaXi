#include	"Player.h"
#include	<DirectXMath.h>
#include	<string>
#include	"../../system/dx11/CDirectInput.h"
#include	"../../system/dx11/DX11Settransform.h"

#include	"../component/allcomponents.h"

#include	<iostream>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_MOVE_MODEL	(0.085f)				// �ړ����x
#define	VALUE_ROTATE_MODEL	(XM_PI * 0.025f)		// ��]���x
#define	RATE_ROTATE_MODEL	(0.10f)					// ��]�����W��
#define	RATE_MOVE_MODEL		(0.1f)

Player::~Player()
{
	Uninit();
};

void Player::ObjectInit()
{
	mTransform.ReSetValue();

	mTransform.SetPosition(Float3(mInitMapPos.x*DICE_SCALE, -DICE_SCALE / 2.0f, -mInitMapPos.z*DICE_SCALE));
	mTransform.CreateMtx();

	mDirection = Direction::eDown;

	mIsDiceMove = false;
}

void Player::ObjectUpdate()
{
	switch (mPstate)
	{
	case eStop:
		if (mStartCount < 0)
		{
			mPstate = eMove;
		}
		else
		{
			mStartCount--;
		}
	case eMove:
		Move();
		break;;
	case ePush:
		Push();
		break;
	case eRoll:
		Roll();
		break;
	}
}

void Player::Uninit()
{
}

void Player::OnCollisionEnter(ComponentBase* _oher)
{
	std::cout << "OnCollisionEnter�@ObjectName:" + _oher->GetOwner()->GetName() + "\n";

	if (_oher->GetTag() == ObjectTag::Dice && !mIsDiceOperation)
	{
		OnColEnterObj(dynamic_cast<Dice*>(_oher->GetOwner()));
	}
}

void Player::OnCollisionStay(ComponentBase* _oher)
{
	std::cout << "OnCollisionStay�@ObjectName:" + _oher->GetOwner()->GetName() + "\n";
	if (_oher->GetTag() == ObjectTag::Dice)
	{
		OnColStayObj(dynamic_cast<Dice*>(_oher->GetOwner()));
	}
}

void Player::OnCollisionExit(ComponentBase* _oher)
{
	std::cout << "OnCollisionExit�@ObjectName:" + _oher->GetOwner()->GetName() + "\n";
	if (_oher->GetTag() == ObjectTag::Dice && mIsDiceOperation)
	{
		OnColExitObj(dynamic_cast<Dice*>(_oher->GetOwner()));
	}
}

void Player::OnColEnterObj(Dice* _other)
{
	mOperationDice = _other;
	mIsDiceOperation = true;
	if (mPstate != eMove)
		return;
	if (_other->Push(mDirection))
	{
		// ��Ԃ�ς���
		mPstate = ePush;
	}
}

void Player::OnColStayObj(Dice* _other)
{
}

void Player::OnColExitObj(Dice* _other)
{
	if (mOperationDice == _other)
	{
		mIsDiceOperation = false;
	}
}

void Player::Move()
{
	Float3 rotCamera;
	float radian;
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
	{
		// ���ړ�
		radian = rotCamera.y + XM_PI * 0.50f;
		mTransform.move.x -= sinf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
		mTransform.move.z -= cosf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
		// �ڕW�p�x���Z�b�g
		mDestrot.y = radian;
		mDirection = Direction::eLeft;
	}
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
	{
		// �E�ړ�
		radian = rotCamera.y - XM_PI * 0.50f;
		mTransform.move.x -= sinf(radian) * VALUE_MOVE_MODEL;
		mTransform.move.z -= cosf(radian) * VALUE_MOVE_MODEL;
		// �ڕW�p�x���Z�b�g
		mDestrot.y = radian;
		mDirection = Direction::eRight;
	}
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
	{
		// �O�ړ�
		mTransform.move.x -= sinf(XM_PI) * VALUE_MOVE_MODEL;
		mTransform.move.z -= cosf(XM_PI) * VALUE_MOVE_MODEL;
		// �ڕW�p�x���Z�b�g
		mDestrot.y = rotCamera.y + XM_PI;
		mDirection = Direction::eUp;
	}
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
	{
		// ��ړ�
		mTransform.move.x -= sinf(rotCamera.y) * VALUE_MOVE_MODEL;
		mTransform.move.z -= cosf(rotCamera.y) * VALUE_MOVE_MODEL;
		// �ڕW�p�x���Z�b�g
		mDestrot.y = rotCamera.y;
		mDirection = Direction::eDown;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RETURN))
	{
		mTransform.ReSetValue();
	}
	// �ڕW�p�x�ƌ��݊p�x�Ƃ̍��������߂�
	float diffrot = mDestrot.y - mTransform.rotation.y;
	if (diffrot > XM_PI)
	{
		diffrot -= XM_PI * 2.0f;
	}
	if (diffrot < -XM_PI)
	{
		diffrot += XM_PI * 2.0f;
	}

	// �p�x���x�Ɋ�����t����
	mTransform.rotation.y += diffrot * RATE_ROTATE_MODEL;
	if (mTransform.rotation.y > XM_PI)
	{
		mTransform.rotation.y -= XM_PI * 2.0f;
	}
	if (mTransform.rotation.y < -XM_PI)
	{
		mTransform.rotation.y += XM_PI * 2.0f;
	}

	// �ʒu�ړ�
	mTransform.AddPosition();
	// �ړ��ʂɊ�����������(��������)
	mTransform.move += (mTransform.move*-1.0f) * RATE_MOVE_MODEL;
	// ��]�𔽉f�A���s�ړ��𔽉f
	mTransform.angle = ((mTransform.rotation* 180.0f) / XM_PI);

	//mTransform.position.y = DICESCALE / 2.0f - 3;

	mTransform.CreateMtx();
}

void Player::Roll()
{
}

void Player::Push()
{
	if (mOperationDice->GetPush())
	{
		mPstate = eMove;
		mTransform.move = mTransform.move * -1.0f;
		mTransform.AddPosition();
		mTransform.CreateMtx();
		mTransform.move = 0;
		return;
	}

	float movePos = mOperationDice->GetmPushPositionPerFrame();

	switch (mDirection)
	{
	case Direction::eNeutral:
		mPstate = eMove;
		return;
	case Direction::eUp:
		std::cout << "�㋭���ړ�\n";
		mTransform.move = { 0, 0, movePos };
		break;
	case Direction::eDown:
		std::cout << "�������ړ�\n";
		mTransform.move = { 0, 0, -movePos };
		break;
	case Direction::eLeft:
		std::cout << "�������ړ�\n";
		mTransform.move = { -movePos, 0, 0 };
		break;
	case Direction::eRight:
		std::cout << "�E�����ړ�\n";
		mTransform.move = { movePos, 0, 0 };
		break;
	}

	mTransform.AddPosition();
	mTransform.CreateMtx();
}
