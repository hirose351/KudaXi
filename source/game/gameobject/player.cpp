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
	switch (mPstate)
	{
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
	std::cout << "OnCollisionEnter　ObjectName:" + _oher->GetOwner()->GetName() + "\n";

	if (_oher->GetTag() == ObjectTag::Dice && !isDiceOperation)
	{
		OnColEnterObj(dynamic_cast<Dice*>(_oher->GetOwner()));
	}
}

void Player::OnCollisionStay(ComponentBase* _oher)
{
	std::cout << "OnCollisionStay　ObjectName:" + _oher->GetOwner()->GetName() + "\n";
	if (_oher->GetTag() == ObjectTag::Dice)
	{
		OnColStayObj(dynamic_cast<Dice*>(_oher->GetOwner()));
	}
}

void Player::OnCollisionExit(ComponentBase* _oher)
{
	std::cout << "OnCollisionExit　ObjectName:" + _oher->GetOwner()->GetName() + "\n";
	if (_oher->GetTag() == ObjectTag::Dice && isDiceOperation)
	{
		OnColExitObj(dynamic_cast<Dice*>(_oher->GetOwner()));
	}
}

void Player::OnColEnterObj(Dice* _other)
{
	mDice = _other;

	if (_other->Push(mDirection))
	{
		// 状態を変える
		mPstate = ePush;
	}
	isDiceOperation = true;
}

void Player::OnColStayObj(Dice* _other)
{
}

void Player::OnColExitObj(Dice* _other)
{
	if (mDice == _other)
	{
		isDiceOperation = false;
	}
}

void Player::Move()
{
	Float3 rotCamera(0, 0, 0);
	float radian;
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
	{
		// 左移動
		radian = rotCamera.y + XM_PI * 0.50f;

		mTransform.move.x -= sinf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
		mTransform.move.z -= cosf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
		// 目標角度をセット
		mDestrot.y = radian;
		mDirection = DIRECTION::LEFT;
	}
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
	{
		// 右移動
		radian = rotCamera.y - XM_PI * 0.50f;

		mTransform.move.x -= sinf(radian) * VALUE_MOVE_MODEL;
		mTransform.move.z -= cosf(radian) * VALUE_MOVE_MODEL;
		// 目標角度をセット
		mDestrot.y = radian;
		mDirection = DIRECTION::RIGHT;
	}
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
	{
		// 前移動
		mTransform.move.x -= sinf(XM_PI) * VALUE_MOVE_MODEL;
		mTransform.move.z -= cosf(XM_PI) * VALUE_MOVE_MODEL;
		// 目標角度をセット
		mDestrot.y = rotCamera.y + XM_PI;
		mDirection = DIRECTION::UP;
	}
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
	{
		// 後移動
		mTransform.move.x -= sinf(rotCamera.y) * VALUE_MOVE_MODEL;
		mTransform.move.z -= cosf(rotCamera.y) * VALUE_MOVE_MODEL;
		// 目標角度をセット
		mDestrot.y = rotCamera.y;
		mDirection = DIRECTION::DOWN;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RETURN))
	{
		mTransform.ReSetValue();
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
	mTransform.AddPosition();
	// 移動量に慣性をかける(減速慣性)
	mTransform.move += (mTransform.move*-1.0f) * RATE_MOVE_MODEL;
	// 回転を反映、平行移動を反映
	mTransform.angle = ((mTransform.rotation* 180.0f) / XM_PI);

	mTransform.position.y = DICESCALE / 2.0f - 3;

	mTransform.CreateMtx();
}

void Player::Roll()
{
}

void Player::Push()
{
	if (mDice->GetPush())
	{
		mPstate = eMove;
		mTransform.move = mTransform.move * -1.0f;
		mTransform.AddPosition();
		mTransform.CreateMtx();
		mTransform.move = 0;
		return;
	}

	float movePos = mDice->GetmPushPositionPerFrame();

	switch (mDirection)
	{
	case DIRECTION::NEUTRAL:
		mPstate = eMove;
		return;
	case DIRECTION::UP:
		std::cout << "上強制移動\n";
		mTransform.move = { 0, 0, movePos };
		break;
	case DIRECTION::DOWN:
		std::cout << "下強制移動\n";
		mTransform.move = { 0, 0, -movePos };
		break;
	case DIRECTION::LEFT:
		std::cout << "左強制移動\n";
		mTransform.move = { -movePos, 0, 0 };
		break;
	case DIRECTION::RIGHT:
		std::cout << "右強制移動\n";
		mTransform.move = { movePos, 0, 0 };
		break;
	}

	mTransform.AddPosition();
	mTransform.CreateMtx();
}
