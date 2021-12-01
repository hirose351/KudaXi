#include	"Player.h"
#include	<DirectXMath.h>
#include	<string>
#include	"../../system/dx11/CDirectInput.h"
#include	"../../system/dx11/DX11Settransform.h"

#include	"../component/allcomponents.h"

#include	<iostream>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_MOVE_MODEL	(0.085f)				// 移動速度
#define	VALUE_ROTATE_MODEL	(XM_PI * 0.025f)		// 回転速度
#define	RATE_ROTATE_MODEL	(0.10f)					// 回転慣性係数
#define	RATE_MOVE_MODEL		(0.1f)
//#define	RATE_MOVE_MODEL		(0.025f)			// 移動慣性係数

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
			//std::cout << "左\n";

			// 左移動
			radian = rotCamera.y + XM_PI * 0.50f;

			mTransform.move.x -= sinf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
			mTransform.move.z -= cosf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
			//// z軸調整
			//if (m_pos.z > (-m_mapPos.z*DICESCALE)) {
			//	m_move.z -= 0.01f;
			//}
			//else {
			//	m_move.z += 0.01f;
			//}
			// 目標角度をセット
			mDestrot.y = radian;
			mDirection = DIRECTION::LEFT;
		}
		else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
		{
			//std::cout << "右\n";
			// 右移動
			radian = rotCamera.y - XM_PI * 0.50f;

			mTransform.move.x -= sinf(radian) * VALUE_MOVE_MODEL;
			mTransform.move.z -= cosf(radian) * VALUE_MOVE_MODEL;
			//// z軸調整
			//if (m_pos.z > (-m_mapPos.z*DICESCALE)) {
			//	m_move.z -= 0.01f;
			//}
			//else {
			//	m_move.z += 0.01f;
			//}
			// 目標角度をセット
			mDestrot.y = radian;
			mDirection = DIRECTION::RIGHT;
		}
		else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
		{
			//std::cout << "上\n";
			// 前移動
			mTransform.move.x -= sinf(XM_PI) * VALUE_MOVE_MODEL;
			mTransform.move.z -= cosf(XM_PI) * VALUE_MOVE_MODEL;
			//// x軸調整
			//if (m_pos.x > m_mapPos.x*DICESCALE) {
			//	m_move.x -= 0.01f;
			//}
			//else {
			//	m_move.x += 0.01f;
			//}
			// 目標角度をセット
			mDestrot.y = rotCamera.y + XM_PI;
			mDirection = DIRECTION::UP;
		}
		else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
		{
			//std::cout << "下\n";
			// 後移動
			mTransform.move.x -= sinf(rotCamera.y) * VALUE_MOVE_MODEL;
			mTransform.move.z -= cosf(rotCamera.y) * VALUE_MOVE_MODEL;
			//// x軸調整
			//if (m_pos.x > m_mapPos.x*DICESCALE) {
			//	m_move.x -= 0.01f;
			//}
			//else {
			//	m_move.x += 0.01f;
			//}
			// 目標角度をセット
			mDestrot.y = rotCamera.y;
			mDirection = DIRECTION::DOWN;
		}

		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RETURN))
		{
			mTransform.ReSetValue();
		}
	}
	// 目標角度と現在角度との差分を求める
	float diffrot = mDestrot.y - mTransform.rotation.y;
	if (diffrot > XM_PI)
	{
		diffrot -= XM_PI * 2.0f;
	}
	if (diffrot < -XM_PI)
	{
		diffrot += XM_PI * 2.0f;
	}

	// 角度速度に慣性を付ける
	mTransform.rotation.y += diffrot * RATE_ROTATE_MODEL;
	if (mTransform.rotation.y > XM_PI)
	{
		mTransform.rotation.y -= XM_PI * 2.0f;
	}
	if (mTransform.rotation.y < -XM_PI)
	{
		mTransform.rotation.y += XM_PI * 2.0f;
	}

	// 位置移動
	mTransform.position += mTransform.move;
	// 移動量に慣性をかける(減速慣性)
	mTransform.move += (mTransform.move*-1.0f) * RATE_MOVE_MODEL;
	// 回転を反映、平行移動を反映
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
	std::cout << "OnCollisionEnter　ObjectName:" + _oher->GetOwner()->GetName() + "\n";

	if (_oher->GetTag() == ObjectTag::Dice)
	{
		OnColEnterObj(dynamic_cast<Dice*>(_oher->GetOwner()));
	}
}

void Player::OnCollisionStay(ComponentBase* _oher)
{
	std::cout << "OnCollisionStay　ObjectName:" + _oher->GetOwner()->GetName() + "\n";
}

void Player::OnCollisionExit(ComponentBase* _oher)
{
	std::cout << "OnCollisionExit　ObjectName:" + _oher->GetOwner()->GetName() + "\n";
}

void Player::OnColEnterObj(Dice * _other)
{
	_other->Push(mDirection);
}
