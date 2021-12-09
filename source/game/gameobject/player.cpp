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

	mTransform.SetPosition(Float3(mInitMapPos.x*DICE_SCALE, DICE_SCALE_HALF, -mInitMapPos.z*DICE_SCALE));
	mTransform.CreateMtx();

	mDirection = Direction::eDown;

	mIsDiceMove = false;
	stageData.SetStageData(StageDataManager::GetInstance().GetCurrentStage());
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
		break;
	case eMove:
		// 重力仮
		//mTransform.PositionCorrectionY(-1);
		Move();
		CheckRoll();
		break;
	case ePush:
		Push();
		break;
	case eRoll:
		Roll();
		break;
	}

	if (mpOperationDice == nullptr)
		return;
	// 最も近いサイコロを検索
	SetNearestDice();
	// ステージに合わせて位置修正
	StageHitCorrection();
}

void Player::ObjectImguiDraw()
{
	std::string str;
	if (mpOperationDice == nullptr)
	{
		str = "null";
	}
	else
	{
		str = mpOperationDice->GetName();
	}
	ImGui::Text(str.c_str());
}

void Player::Uninit()
{
}

void Player::OnCollisionEnter(ComponentBase* _oher)
{
	std::cout << "OnCollisionEnter　ObjectName:" + _oher->GetOwner()->GetName() + "\n";

	if ((_oher->GetTag() == ObjectTag::Dice || _oher->GetTag() == ObjectTag::DiceTop))
	{
		OnColEnterObj(dynamic_cast<Dice*>(_oher->GetOwner()));
	}
}

void Player::OnCollisionStay(ComponentBase* _oher)
{
	std::cout << "OnCollisionStay　ObjectName:" + _oher->GetOwner()->GetName() + "\n";
	if (_oher->GetTag() == ObjectTag::Dice || _oher->GetTag() == ObjectTag::DiceTop)
	{
		OnColStayObj(dynamic_cast<Dice*>(_oher->GetOwner()));
	}
}

void Player::OnCollisionExit(ComponentBase* _oher)
{
	std::cout << "OnCollisionExit　ObjectName:" + _oher->GetOwner()->GetName() + "\n";
	if ((_oher->GetTag() == ObjectTag::Dice || _oher->GetTag() == ObjectTag::DiceTop) && mIsDiceOperation)
	{
		OnColExitObj(dynamic_cast<Dice*>(_oher->GetOwner()));
	}
}

void Player::OnColEnterObj(Dice* _other)
{
	//mpOperationDice = _other;
	//mIsDiceOperation = true;
	if (mPstate != eMove)
		return;
	if (mTransform.GetPosition().y > DICE_SCALE_HALF)
		return;
	if (_other->SetPushAction(mDirection))
		mPstate = ePush;	// 状態を変える
}

void Player::OnColStayObj(Dice* _other)
{
	// 最も近いサイコロを検索
	SetNearestDice();

	//if (mpOperationDice != nullptr)
	//	return;
	//mpOperationDice = _other;
}

void Player::OnColExitObj(Dice* _other)
{
	if (mpOperationDice == _other && DICE_SCALE_HALF > mTransform.position.y)
	{
		mpOperationDice = nullptr;
		mIsDiceOperation = false;
	}
}

void Player::Move()
{
	Float3 rotCamera;
	float radian;
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
	{
		// 左移動
		radian = rotCamera.y + XM_PI * 0.50f;
		mTransform.move.x -= sinf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
		mTransform.move.z -= cosf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
		// 目標角度をセット
		mDestrot.y = radian;
		mDirection = Direction::eLeft;
	}
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
	{
		// 右移動
		radian = rotCamera.y - XM_PI * 0.50f;
		mTransform.move.x -= sinf(radian) * VALUE_MOVE_MODEL;
		mTransform.move.z -= cosf(radian) * VALUE_MOVE_MODEL;
		// 目標角度をセット
		mDestrot.y = radian;
		mDirection = Direction::eRight;
	}
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
	{
		// 前移動
		mTransform.move.x -= sinf(XM_PI) * VALUE_MOVE_MODEL;
		mTransform.move.z -= cosf(XM_PI) * VALUE_MOVE_MODEL;
		// 目標角度をセット
		mDestrot.y = rotCamera.y + XM_PI;
		mDirection = Direction::eUp;
	}
	else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
	{
		// 後移動
		mTransform.move.x -= sinf(rotCamera.y) * VALUE_MOVE_MODEL;
		mTransform.move.z -= cosf(rotCamera.y) * VALUE_MOVE_MODEL;
		// 目標角度をセット
		mDestrot.y = rotCamera.y;
		mDirection = Direction::eDown;
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

	//mTransform.position.y = DICESCALE / 2.0f - 3;

	mTransform.CreateMtx();
}

void Player::Roll()
{
	if (mpOperationDice == nullptr)
	{
		mPstate = eMove;
		return;
	}
	if (mpOperationDice->GetRollEnd())
	{
		mPstate = eMove;
		mTransform.move = mTransform.move * -1.0f;
		mTransform.AddPosition();
		mTransform.CreateMtx();
		mTransform.move = 0;
		return;
	}

	mTransform.move = 0;
	float movePos = mpOperationDice->GetmPushPositionPerFrame() / 1.5f;

	switch (mDirection)
	{
	case Direction::eNeutral:
		mPstate = eMove;
		return;
	case Direction::eUp:
		std::cout << "上強制移動\n";
		mTransform.move = { 0, 0, movePos };
		break;
	case Direction::eDown:
		std::cout << "下強制移動\n";
		mTransform.move = { 0, 0, -movePos };
		break;
	case Direction::eLeft:
		std::cout << "左強制移動\n";
		mTransform.move = { -movePos, 0, 0 };
		break;
	case Direction::eRight:
		std::cout << "右強制移動\n";
		mTransform.move = { movePos, 0, 0 };
		break;
	}

	mTransform.AddPosition();
	mTransform.CreateMtx();
}

void Player::Push()
{
	if (mpOperationDice->GetPushEnd())
	{
		mPstate = eMove;
		mTransform.move = mTransform.move * -1.0f;
		mTransform.AddPosition();
		mTransform.CreateMtx();
		mTransform.move = 0;
		return;
	}

	float movePos = 1;

	switch (mDirection)
	{
	case Direction::eNeutral:
		mPstate = eMove;
		return;
	case Direction::eUp:
		std::cout << "上強制移動\n";
		mTransform.move = { 0, 0, movePos };
		break;
	case Direction::eDown:
		std::cout << "下強制移動\n";
		mTransform.move = { 0, 0, -movePos };
		break;
	case Direction::eLeft:
		std::cout << "左強制移動\n";
		mTransform.move = { -movePos, 0, 0 };
		break;
	case Direction::eRight:
		std::cout << "右強制移動\n";
		mTransform.move = { movePos, 0, 0 };
		break;
	}

	mTransform.AddPosition();
	mTransform.CreateMtx();
}

void Player::CheckRoll()
{
	if (mpOperationDice == nullptr)
		return;

	Float3 basePoint = mpOperationDice->GetTransform()->GetPosition();	// 基準点

	switch (mDirection)
	{
	case Direction::eNeutral:
		return;
	case Direction::eUp:
		if (basePoint.z + DICE_SCALE_HALF >= mTransform.position.z)
			return;
		break;
	case Direction::eDown:
		if (basePoint.z - DICE_SCALE_HALF <= mTransform.position.z)
			return;
		break;
	case Direction::eLeft:
		if (basePoint.x - DICE_SCALE_HALF <= mTransform.position.x)
			return;
		break;
	case Direction::eRight:
		if (basePoint.x + DICE_SCALE_HALF >= mTransform.position.x)
			return;
		break;
	}

	// 回転可能位置にいたら
	if (mpOperationDice->SetRollAction(mDirection))
		mPstate = eRoll;// 状態を変える
}

void Player::StageHitCorrection()
{
	// 壁の処理
	if (stageData.mMapSizeHeight*-DICE_SCALE - DICE_SCALE_HALF > mTransform.position.z)
		mTransform.SetPositionZ(stageData.mMapSizeHeight*-DICE_SCALE - DICE_SCALE_HALF);
	if (0 + DICE_SCALE_HALF < mTransform.position.z)
		mTransform.SetPositionZ(0 + DICE_SCALE_HALF);
	if (stageData.mMapSizeWidth*DICE_SCALE - DICE_SCALE_HALF < mTransform.position.x)
		mTransform.SetPositionX(stageData.mMapSizeWidth*DICE_SCALE - DICE_SCALE_HALF);
	if (0 - DICE_SCALE_HALF > mTransform.position.x)
		mTransform.SetPositionX(0 - DICE_SCALE_HALF);

	// サイコロの処理

}

bool Player::SetNearestDice()
{
	mpOperationDice = dynamic_cast<Dice*>(GetComponent<Component::CollisionComponent>()->GetNearestDice(mTransform.position));

	if (mpOperationDice == nullptr)
		return false;
	//std::cout << "保持Dice基準Y補正\n";
	//mTransform.SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE_HALF + 4 - 0.5f);
	return true;
}
