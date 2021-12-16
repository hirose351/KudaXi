#include	"Player.h"
#include	<DirectXMath.h>
#include	<string>
#include	"../../system/dx11/CDirectInput.h"
#include	"../../system/dx11/DX11Settransform.h"
#include	"../component/allcomponents.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_MOVE_MODEL	(0.085f)				// 移動速度
#define	VALUE_ROTATE_MODEL	(XM_PI * 0.025f)		// 回転速度
#define	RATE_ROTATE_MODEL	(0.10f)					// 回転慣性係数
#define	RATE_MOVE_MODEL		(0.1f)


bool hitcheck(Float3 p, Float3 qp, float qsizex, float qsizez) {
	if (p.x >= qp.x&&p.x <= (qp.x + qsizex) && p.z >= qp.z&&p.z <= (qp.z + qsizez))
		return true;
	return false;
}

Player::~Player()
{
	Uninit();
};

void Player::ObjectInit()
{
	mTransform.ReSetValue();

	mFoot = Foot::eFloor;
	mDirection = Direction::eDown;

	stageData.SetStageData(StageDataManager::GetInstance().GetCurrentStage());

	mTransform.SetPosition(Float3(1 * DICE_SCALE, DICE_SCALE_HALF, -1 * DICE_SCALE));
	//mTransform.SetPosition(Float3(stageData.mPlayerPos.x*DICE_SCALE, DICE_SCALE_HALF, -stageData.mPlayerPos.z*DICE_SCALE));
	mTransform.CreateMtx();
	mPstate = eMove;
}

void Player::ObjectUpdate()
{
	switch (mPstate)
	{
	case eStop:
		break;
	case eMove:
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

	// ステージに合わせて位置修正
	StageHitCorrection();
}

void Player::ObjectImguiDraw()
{
	std::string str;
	if (mpOperationDice == nullptr)
	{
		str = "HaveDice:null";
	}
	else
	{
		str = "HaveDice:" + mpOperationDice->GetName();
	}
	ImGui::Text(str.c_str());

	str = FootStr[static_cast<int>(mFoot)];
	ImGui::Text(str.c_str());
}

void Player::Uninit()
{
}


void Player::OnColEnterObj(Dice* _other)
{
	// 最も近いサイコロを検索
	SetNearestDice();

	if (mPstate != eMove || mFoot != Foot::eFloor)
		return;
	if (mTransform.GetPosition().y > DICE_SCALE_HALF)
		return;
	if (!_other->SetPushAction(mDirection))
		return;
	mpOperationDice = _other;
	mPstate = ePush;	// 状態を変える
}

void Player::OnColStayObj(Dice* _other)
{
	// 最も近いサイコロを検索
	SetNearestDice();
}

void Player::OnColExitObj(Dice* _other)
{
	if (mpOperationDice == _other && DICE_SCALE_HALF > mTransform.position.y&&mPstate != ePush)
	{
		mpOperationDice = nullptr;
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

	// 重力仮
	mTransform.PositionCorrectionY(-1);
	if (mTransform.position.y < DICE_SCALE_HALF - 3)
	{
		mTransform.position.y = DICE_SCALE_HALF - 3;
	}

	mTransform.CreateMtx();
}

void Player::Roll()
{
	if (mpOperationDice == nullptr)
	{
		mPstate = eMove;
		return;
	}
	Float3 dicePos = mpOperationDice->GetTransform()->GetPosition();

	switch (mDirection)
	{
	case Direction::eNeutral:
		mPstate = eMove;
		return;
	case Direction::eUp:
		//std::cout << "上強制移動\n";
		mTransform.position.z = dicePos.z + DICE_SCALE_HALF;
		break;
	case Direction::eDown:
		//std::cout << "下強制移動\n";
		mTransform.position.z = dicePos.z - DICE_SCALE_HALF;
		break;
	case Direction::eLeft:
		//std::cout << "左強制移動\n";
		mTransform.position.x = dicePos.x - DICE_SCALE_HALF;
		break;
	case Direction::eRight:
		//std::cout << "右強制移動\n";
		mTransform.position.x = dicePos.x + DICE_SCALE_HALF;
		break;
	}
	mTransform.CreateMtx();

	if (mpOperationDice->GetDiceStatus() != DiceStatus::eRoll)
	{
		mPstate = eMove;
		mDirection = Direction::eNeutral;
		mTransform.move = 0;
	}
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
		//std::cout << "上強制移動\n";
		mTransform.move = { 0, 0, movePos };
		break;
	case Direction::eDown:
		//std::cout << "下強制移動\n";
		mTransform.move = { 0, 0, -movePos };
		break;
	case Direction::eLeft:
		//std::cout << "左強制移動\n";
		mTransform.move = { -movePos, 0, 0 };
		break;
	case Direction::eRight:
		//std::cout << "右強制移動\n";
		mTransform.move = { movePos, 0, 0 };
		break;
	}

	mTransform.AddPosition();
	mTransform.CreateMtx();
}

void Player::CheckRoll()
{
	if (mpOperationDice == nullptr || mFoot != Foot::eDice)
		return;
	if (mpOperationDice->GetDiceStatus() != DiceStatus::eNormal)
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
	{
		mPstate = eRoll;	// 状態を変える
		mTransform.SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE_HALF + 6 - 0.5f);
	}
	// 回転出来なければ
	else
	{
		/// Todo:行き先が穴だった時の移動制限
		if (!mpOperationDice->CheckDiceDirection(mDirection))
			return;
		switch (mDirection)
		{
		case Direction::eUp:
			mTransform.SetPositionZ(basePoint.z + DICE_SCALE_HALF);
			break;
		case Direction::eDown:
			mTransform.SetPositionZ(basePoint.z - DICE_SCALE_HALF);
			break;
		case Direction::eLeft:
			mTransform.SetPositionX(basePoint.x - DICE_SCALE_HALF);
			break;
		case Direction::eRight:
			mTransform.SetPositionX(basePoint.x + DICE_SCALE_HALF);
			break;
		}
	}
}

void Player::StageHitCorrection()
{
	// 上下左右壁の処理
	if (stageData.mMapSizeHeight*-DICE_SCALE + DICE_SCALE_HALF > mTransform.position.z)
		mTransform.SetPositionZ(stageData.mMapSizeHeight*-DICE_SCALE + DICE_SCALE_HALF);
	if (0 + DICE_SCALE_HALF < mTransform.position.z)
		mTransform.SetPositionZ(0 + DICE_SCALE_HALF);
	if (stageData.mMapSizeWidth*DICE_SCALE - DICE_SCALE_HALF < mTransform.position.x)
		mTransform.SetPositionX(stageData.mMapSizeWidth*DICE_SCALE - DICE_SCALE_HALF);
	if (0 - DICE_SCALE_HALF > mTransform.position.x)
		mTransform.SetPositionX(0 - DICE_SCALE_HALF);
}

bool Player::SetNearestDice()
{
	if (mPstate == ePush || mPstate == eRoll)
		return false;

	mpOperationDice = dynamic_cast<Dice*>(GetComponent<Component::CollisionComponent>()->GetNearestDice(mTransform.position));

	if (mpOperationDice == nullptr)
	{
		mFoot = Foot::eFloor;
		return false;
	}

	if (mTransform.GetPosition().y <= DICE_SCALE_HALF)
		mFoot = Foot::eFloor;

	if (mpOperationDice->GetTransform()->GetPosition().y < mTransform.position.y)
		mFoot = Foot::eDice;

	if (mFoot == Foot::eFloor)
	{
		if (mpOperationDice->GetDiceStatus() != DiceStatus::eNormal)
		{
			//std::cout << "保持Dice基準Y補正\n";
			mTransform.SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE_HALF + 6 - 0.5f);
		}
	}
	else if (mFoot == Foot::eDice)
	{
		Float3 dicePos = (mpOperationDice->GetTransform()->position);
		switch (mpOperationDice->GetDiceStatus())
		{
		case DiceStatus::eUp:
			//std::cout << "保持Dice基準Y補正\n";
			mTransform.SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE_HALF + 6 - 0.5f);
			break;
		case DiceStatus::eHalfUp:
			// 移動制限
			if (dicePos.x + DICE_SCALE_HALF - 2 < mTransform.GetPosition().x)
				mTransform.SetPositionX(dicePos.x + DICE_SCALE_HALF - 2);
			if (dicePos.x - DICE_SCALE_HALF + 2 > mTransform.GetPosition().x)
				mTransform.SetPositionX(dicePos.x - DICE_SCALE_HALF + 2);
			if (dicePos.z + DICE_SCALE_HALF - 2 < mTransform.GetPosition().z)
				mTransform.SetPositionZ(dicePos.z + DICE_SCALE_HALF - 2);
			if (dicePos.z - DICE_SCALE_HALF + 2 > mTransform.GetPosition().z)
				mTransform.SetPositionZ(dicePos.z - DICE_SCALE_HALF + 2);
			//std::cout << "保持Dice基準Y補正\n";
			mTransform.SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE_HALF + 6 - 0.5f);
			break;
		case DiceStatus::eRoll:
			break;
		case DiceStatus::ePush:
			//std::cout << "保持Dice基準Y補正\n";
			break;
		case DiceStatus::eDown:
			//std::cout << "保持Dice基準Y補正\n";
			mTransform.SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE_HALF + 6 - 0.5f);
			break;
		case DiceStatus::eHalfDown:
			//std::cout << "保持Dice基準Y補正\n";
			mTransform.SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE_HALF + 6 - 0.5f);
			break;
		default:
			//std::cout << "保持Dice基準Y補正\n";
			mTransform.SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE_HALF + 6 - 0.5f);
			break;
		}
		mTransform.SetPositionY(mpOperationDice->GetTransform()->GetPosition().y + DICE_SCALE_HALF + 6 - 0.5f);
	}
	return true;
}

void Player::OnCollisionEnter(ComponentBase* _oher)
{
	//std::cout << "OnCollisionEnter　ObjectName:" + _oher->GetOwner()->GetName() + "\n";

	if ((_oher->GetTag() == ObjectTag::eDice || _oher->GetTag() == ObjectTag::eDiceTop))
	{
		OnColEnterObj(dynamic_cast<Dice*>(_oher->GetOwner()));
	}
}

void Player::OnCollisionStay(ComponentBase* _oher)
{
	//std::cout << "OnCollisionStay　ObjectName:" + _oher->GetOwner()->GetName() + "\n";
	if (_oher->GetTag() == ObjectTag::eDice || _oher->GetTag() == ObjectTag::eDiceTop)
	{
		OnColStayObj(dynamic_cast<Dice*>(_oher->GetOwner()));
	}
}

void Player::OnCollisionExit(ComponentBase* _oher)
{
	//std::cout << "OnCollisionExit　ObjectName:" + _oher->GetOwner()->GetName() + "\n";
	if ((_oher->GetTag() == ObjectTag::eDice || _oher->GetTag() == ObjectTag::eDiceTop))
	{
		OnColExitObj(dynamic_cast<Dice*>(_oher->GetOwner()));
	}
}