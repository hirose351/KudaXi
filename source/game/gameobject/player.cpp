#include	"Player.h"
#include	<DirectXMath.h>
#include	<string>
#include	"../../system/dx11/CDirectInput.h"
#include	"../../system/dx11/DX11Settransform.h"
#include	"../../system/model/ModelMgr.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_MOVE_MODEL	(0.085f)				// �ړ����x
#define	VALUE_ROTATE_MODEL	(XM_PI * 0.025f)		// ��]���x
#define	RATE_ROTATE_MODEL	(0.10f)					// ��]�����W��
#define	RATE_MOVE_MODEL		(0.1f)
//#define	RATE_MOVE_MODEL		(0.025f)			// �ړ������W��

Player::Player()
{
	bool sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/player/player.fbx",
		"shader/vs.hlsl",
		"shader/ps.hlsl",
		"assets/model/player/");
	//�P�ʍs��
	DX11MtxIdentity(mMtx);
	// �����p��
	mRotation = { 0.0f, 0.0f, 0.0f };
	// �ڕW�p��
	mDestrot = { 0.0f, 0.0f, 0.0f };
	// �ړ���
	mMove = { 0.0f, 0.0f, 0.0f };

	SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/player/player.fbx"));

	Init();
}

Player::~Player()
{
	Finalize();
};

void Player::Init()
{
	// �����p��
	mRotation = { 0.0f, 0.0f, 0.0f };
	// �ڕW�p��
	mDestrot = { 0.0f, 0.0f, 0.0f };
	// �ړ���
	mMove = { 0.0f, 0.0f, 0.0f };

	mDirection = DIRECTION::DOWN;
	mIsDiceMove = false;
}

void Player::Update()
{
	Vector3 rotCamera;
	float radian;
	if (!mIsDiceMove)
	{
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
		{
			// ���ړ�
			radian = rotCamera.y + XM_PI * 0.50f;

			mMove.x -= sinf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
			mMove.z -= cosf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
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
			// �E�ړ�
			radian = rotCamera.y - XM_PI * 0.50f;

			mMove.x -= sinf(radian) * VALUE_MOVE_MODEL;
			mMove.z -= cosf(radian) * VALUE_MOVE_MODEL;
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
			// �O�ړ�
			mMove.x -= sinf(XM_PI) * VALUE_MOVE_MODEL;
			mMove.z -= cosf(XM_PI) * VALUE_MOVE_MODEL;
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
			// ��ړ�
			mMove.x -= sinf(rotCamera.y) * VALUE_MOVE_MODEL;
			mMove.z -= cosf(rotCamera.y) * VALUE_MOVE_MODEL;
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
			Init();
		}
	}
	// �ڕW�p�x�ƌ��݊p�x�Ƃ̍��������߂�
	float diffrot = mDestrot.y - mRotation.y;
	if (diffrot > XM_PI)
	{
		diffrot -= XM_PI * 2.0f;
	}
	if (diffrot < -XM_PI)
	{
		diffrot += XM_PI * 2.0f;
	}

	// �p�x���x�Ɋ�����t����
	mRotation.y += diffrot * RATE_ROTATE_MODEL;
	if (mRotation.y > XM_PI)
	{
		mRotation.y -= XM_PI * 2.0f;
	}
	if (mRotation.y < -XM_PI)
	{
		mRotation.y += XM_PI * 2.0f;
	}

	/// �ʒu�ړ�
	mPosition += mMove;

	// �ړ��ʂɊ�����������(��������)
	mMove += (mMove*-1.0f) * RATE_MOVE_MODEL;

	// ��]�𔽉f�A���s�ړ��𔽉f
	Vector3 degree = (mRotation* 180.0f) / XM_PI;
	DX11MakeWorldMatrix(mMtx, degree, mPosition);
	DirectX::XMFLOAT4X4 scaleMtx;
	DX11MtxScale(1.3f, 1.3f, 1.3f, scaleMtx);
	DX11MtxMultiply(mMtx, scaleMtx, mMtx);
}
void Player::Draw()
{
	mpModel->Draw(mMtx);
}

void Player::Finalize()
{
	//mpModel->Uninit();
}