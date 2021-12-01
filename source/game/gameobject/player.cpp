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
//#define	RATE_MOVE_MODEL		(0.025f)			// �ړ������W��

Player::~Player()
{
	Uninit();
};

void Player::ObjectInit()
{
	mTransform.ReSetValue();

	mDirection = DIRECTION::DOWN;

	mIsDiceMove = false;
}

void Player::ObjectUpdate()
{
	Float3 rotCamera(0, 0, 0);
	float radian;
	if (!mIsDiceMove)
	{
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
		{
			//std::cout << "��\n";

			// ���ړ�
			radian = rotCamera.y + XM_PI * 0.50f;

			mTransform.move.x -= sinf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
			mTransform.move.z -= cosf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
			//// z������
			//if (m_pos.z > (-m_mapPos.z*DICESCALE)) {
			//	m_move.z -= 0.01f;
			//}
			//else {
			//	m_move.z += 0.01f;
			//}
			// �ڕW�p�x���Z�b�g
			mDestrot.y = radian;
			mDirection = DIRECTION::LEFT;
		}
		else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
		{
			//std::cout << "�E\n";
			// �E�ړ�
			radian = rotCamera.y - XM_PI * 0.50f;

			mTransform.move.x -= sinf(radian) * VALUE_MOVE_MODEL;
			mTransform.move.z -= cosf(radian) * VALUE_MOVE_MODEL;
			//// z������
			//if (m_pos.z > (-m_mapPos.z*DICESCALE)) {
			//	m_move.z -= 0.01f;
			//}
			//else {
			//	m_move.z += 0.01f;
			//}
			// �ڕW�p�x���Z�b�g
			mDestrot.y = radian;
			mDirection = DIRECTION::RIGHT;
		}
		else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
		{
			//std::cout << "��\n";
			// �O�ړ�
			mTransform.move.x -= sinf(XM_PI) * VALUE_MOVE_MODEL;
			mTransform.move.z -= cosf(XM_PI) * VALUE_MOVE_MODEL;
			//// x������
			//if (m_pos.x > m_mapPos.x*DICESCALE) {
			//	m_move.x -= 0.01f;
			//}
			//else {
			//	m_move.x += 0.01f;
			//}
			// �ڕW�p�x���Z�b�g
			mDestrot.y = rotCamera.y + XM_PI;
			mDirection = DIRECTION::UP;
		}
		else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
		{
			//std::cout << "��\n";
			// ��ړ�
			mTransform.move.x -= sinf(rotCamera.y) * VALUE_MOVE_MODEL;
			mTransform.move.z -= cosf(rotCamera.y) * VALUE_MOVE_MODEL;
			//// x������
			//if (m_pos.x > m_mapPos.x*DICESCALE) {
			//	m_move.x -= 0.01f;
			//}
			//else {
			//	m_move.x += 0.01f;
			//}
			// �ڕW�p�x���Z�b�g
			mDestrot.y = rotCamera.y;
			mDirection = DIRECTION::DOWN;
		}

		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RETURN))
		{
			mTransform.ReSetValue();
		}
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
	mTransform.position += mTransform.move;
	// �ړ��ʂɊ�����������(��������)
	mTransform.move += (mTransform.move*-1.0f) * RATE_MOVE_MODEL;
	// ��]�𔽉f�A���s�ړ��𔽉f
	Float3 degree((mTransform.rotation* 180.0f) / XM_PI);

	mTransform.position.y = DICESCALE / 2.0f - 3;

	DX11MakeWorldMatrix(mTransform.worldMtx, degree, mTransform.position);
}

//void Player::Draw()
//{
//}

void Player::Uninit()
{
	//mpModel->Uninit();
}

void Player::OnCollisionEnter(ComponentBase* _oher)
{
	std::cout << "OnCollisionEnter�@ObjectName:" + _oher->GetOwner()->GetName() + "\n";

	if (_oher->GetTag() == ObjectTag::Dice)
	{
		OnColEnterObj(dynamic_cast<Dice*>(_oher->GetOwner()));
	}
}

void Player::OnCollisionStay(ComponentBase* _oher)
{
	std::cout << "OnCollisionStay�@ObjectName:" + _oher->GetOwner()->GetName() + "\n";
}

void Player::OnCollisionExit(ComponentBase* _oher)
{
	std::cout << "OnCollisionExit�@ObjectName:" + _oher->GetOwner()->GetName() + "\n";
}

void Player::OnColEnterObj(Dice * _other)
{
	_other->Push(mDirection);
}
