#include	"dice.h"
#include	"../manager/dice_manager.h"
//#include	"../../system/util/XAudio2.h"

Dice::~Dice()
{
}

void Dice::ObjectInit()
{
	/// Todo:雷クラス作る(他種類も作りたいため)
	mThunder.LoadTexture("assets/image/effect/thunder/thunder_yellow.png");
	mThunder.SetPos(Float3(mTransform.position.x, mTransform.position.y, mTransform.position.z));
	mThunder.SetScale(XMFLOAT2(100.0f, 200.0f));
	mThunder.SetDivUV(XMFLOAT2(2, 1));
	mThunder.SetUV(XMFLOAT2(0, 0));

	mDirection = Direction::eNeutral;
	SetStartUpPosition();
	SetOverPlane();
}

void Dice::ObjectUpdate()
{
	switch (mSts)
	{
	case DICESTATUS::UP:
		GetComponent<Component::CollisionComponent>()->SetColor(XMFLOAT4(1, 1, 1, 0));
		Up();
		break;
	case DICESTATUS::HALF_UP:
		GetComponent<Component::CollisionComponent>()->SetColor(XMFLOAT4(1, 1, 1, 0.5f));
		Up();
		break;
	case DICESTATUS::ROLL:
		GetComponent<Component::CollisionComponent>()->SetColor(XMFLOAT4(1, 1, 1, 0));
		Roll();
		break;
	case DICESTATUS::PUSH:
		GetComponent<Component::CollisionComponent>()->SetColor(XMFLOAT4(1, 1, 1, 0));
		Push();
		break;
	case DICESTATUS::DOWN:
		GetComponent<Component::CollisionComponent>()->SetColor(XMFLOAT4(1, 0, 0, 0.5f));
		Down();
		break;
	case DICESTATUS::HALFDOWN:
		GetComponent<Component::CollisionComponent>()->SetColor(XMFLOAT4(1, 0, 0, 0.3f));
		Down();
		break;
	default:
		GetComponent<Component::CollisionComponent>()->SetColor(XMFLOAT4(1, 1, 1, 0));
		break;
	}
}

void Dice::ObjectDraw()
{
	if (mThunderAlha <= 0.0f)
		return;
	mThunder.Update();
	mThunder.Render();
}

void Dice::Uninit()
{
}

void Dice::SetHappyOne()
{
	// 出目の数が1でなければ返す
	if (mTopDiceTypeNum != 1)
		return;
	// 1であれば落とす
	SetDownPosition();
}

void Dice::MoveDiceScale(Direction _direction)
{
	switch (_direction)
	{
	case Direction::eUp:
		mTransform.worldMtx._43 += DICE_SCALE;
		break;
	case Direction::eDown:
		mTransform.worldMtx._43 -= DICE_SCALE;
		break;
	case Direction::eLeft:
		mTransform.worldMtx._41 -= DICE_SCALE;
		break;
	case Direction::eRight:
		mTransform.worldMtx._41 += DICE_SCALE;
		break;
	}
}

bool Dice::SetPushAction(Direction _direction)
{
	if (mSts != DICESTATUS::NORMAL)
		return false;
	if (!DiceManager::GetInstance()->CanDiceMove(this, _direction))
		return false;

	mTransform.move = 0;
	switch (_direction)
	{
	case Direction::eUp:
		mTransform.move.z = mPushPositionPerFrame;
		break;
	case Direction::eDown:
		mTransform.move.z = -mPushPositionPerFrame;
		break;
	case Direction::eLeft:
		mTransform.move.x = -mPushPositionPerFrame;
		break;
	case Direction::eRight:
		mTransform.move.x = mPushPositionPerFrame;
		break;
	}
	mSts = DICESTATUS::PUSH;
	mDirection = _direction;
	mCrrentPushCnt = 0;
	return true;
}

bool Dice::SetRollAction(Direction _direction)
{
	if (mSts != DICESTATUS::NORMAL)
		return false;
	if (!DiceManager::GetInstance()->CanDiceMove(this, _direction))
		return false;

	mTransform.angle = 0;
	switch (_direction)
	{
	case Direction::eUp:
		mTransform.angle.x = mRotAnglePerFrame;
		break;
	case Direction::eDown:
		mTransform.angle.x = -mRotAnglePerFrame;
		break;
	case Direction::eLeft:
		mTransform.angle.z = mRotAnglePerFrame;
		break;
	case Direction::eRight:
		mTransform.angle.z = -mRotAnglePerFrame;
		break;
	}
	mSts = DICESTATUS::ROLL;
	mDirection = _direction;
	mCrrentRotCnt = 0;
	DX11MakeWorldMatrix(mMtxFrame, mTransform.angle, XMFLOAT3(0, 0, 0));
	mRotateStartPos = { mTransform.worldMtx._41, mTransform.worldMtx._42,mTransform.worldMtx._43 };
	return true;
}

bool Dice::CheckDiceDirection(Direction _direction)
{
	return DiceManager::GetInstance()->CanDiceMoveCheak(this, _direction);
}

void Dice::SetRollDirection(Direction _direction)
{
	//ニュートラルの時だけキー入力を認める
	if (mDirection == Direction::eNeutral)
	{
		mDirection = _direction;
		switch (_direction)
		{
		case Direction::eUp:
			mTransform.angle.x = mRotAnglePerFrame;
			break;
		case Direction::eDown:
			mTransform.angle.x = -mRotAnglePerFrame;
			break;
		case Direction::eLeft:
			mTransform.angle.z = mRotAnglePerFrame;
			break;
		case Direction::eRight:
			mTransform.angle.z = -mRotAnglePerFrame;
			break;
		}
		mCrrentRotCnt = 0;
		DX11MakeWorldMatrix(mMtxFrame, mTransform.angle, XMFLOAT3(0, 0, 0));
		//開始位置を保存
		mRotateStartPos = { mTransform.worldMtx._41, mTransform.worldMtx._42,mTransform.worldMtx._43 };
	}
}

void Dice::SetStartUpPosition()
{
	mTransform.move.y = mUpPositionPerFrame;
	mSts = DICESTATUS::HALF_UP;
}

void Dice::SetDownPosition()
{
	mTransform.move = 0;
	mTransform.move.y = -mUpPositionPerFrame;
	mSts = DICESTATUS::DOWN;
}

void Dice::Push()
{
	mTransform.MovePosition();
	mCrrentPushCnt++;
	if (mCrrentPushCnt >= mMoveCnt)
	{
		mDirection = Direction::eNeutral;
		mSts = DICESTATUS::NORMAL;
		/// 接しているブロックと面が同じかチェック
		DiceManager::GetInstance()->CheckAligned(this);
		/// ステップ数減らす
	}
}

void Dice::Roll()
{
	//行列を作成(ワールドの軸を中心に回転)
	DX11MtxMultiply(mTransform.worldMtx, mTransform.worldMtx, mMtxFrame);

	//半径を計算
	const static float radius = static_cast<float>(DICE_SCALE_HALF*sqrt(2));		// DICESCALE*ルート2/2＝DICESCALE/2.0f*ルート2

	//45度から回転角度を足し算
	float nowcenterposy = radius * sin(ToRad(45 + mRotAnglePerFrame * mCrrentRotCnt));

	//移動量の計算
	Float3 pos = { mRotateStartPos.x,nowcenterposy,mRotateStartPos.z };
	Float3 endpos;
	//割合を計算
	float t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mMoveCnt);

	switch (mDirection)
	{
	case Direction::eUp:
		//終了位置を計算
		endpos.z = mRotateStartPos.z + DICE_SCALE;
		//線形補間の式でX座標を計算
		pos.z = mRotateStartPos.z *(1.0f - t) + endpos.z*t;
		break;
	case Direction::eDown:
		endpos.z = mRotateStartPos.z - DICE_SCALE;
		pos.z = mRotateStartPos.z *(1.0f - t) + endpos.z*t;
		break;
	case Direction::eLeft:
		endpos.x = mRotateStartPos.x - DICE_SCALE;
		pos.x = mRotateStartPos.x *(1.0f - t) + endpos.x*t;
		break;
	case Direction::eRight:
		endpos.x = mRotateStartPos.x + DICE_SCALE;
		pos.x = mRotateStartPos.x *(1.0f - t) + endpos.x*t;
		break;
	}

	//原点の位置を補正
	mTransform.SetPositionXYZ(pos);
	//回転数をカウントアップ
	mCrrentRotCnt++;

	// 回転が終わったら元の状態に戻す
	if (mCrrentRotCnt >= mMoveCnt)
	{
		// 回転後の面に設定
		SetOverPlane();
		mTransform.worldMtx._42 = mTransform.position.y = DICE_SCALE_HALF;
		mDirection = Direction::eNeutral;
		mSts = DICESTATUS::NORMAL;
		mCrrentRotCnt = 0;
		/// 接しているブロックと面が同じかチェック
		DiceManager::GetInstance()->CheckAligned(this);
		/// ステップ数減らす
	}
}

void Dice::Up()
{
	mTransform.MovePosition();
	mCrrentPushCnt++;
	if (mCrrentPushCnt >= mUpCnt)
	{
		mDirection = Direction::eNeutral;
		mSts = DICESTATUS::NORMAL;
	}
	else if (mCrrentPushCnt >= mUpCnt / 2)
	{
		mDirection = Direction::eNeutral;
		mSts = DICESTATUS::UP;
	}

	if (mThunderAlha == 0.5f)
	{
		mThunder.SetUV(XMFLOAT2(1, 0));
	}
	if (mThunderAlha > 0.0f)
	{
		mThunder.SetColor(XMFLOAT4(1, 1, 1, mThunderAlha));
		mThunderAlha -= 0.025f;
	}
}

void Dice::Down()
{
	mTransform.MovePosition();
	if (mTransform.position.y < -DICE_SCALE_HALF)
	{
		DiceManager::GetInstance()->SetRemoveDice(this);
	}
	else if (mTransform.position.y < 0)
	{
		mDirection = Direction::eNeutral;
		mSts = DICESTATUS::HALFDOWN;
	}
}

//増した方向の面を特定
void Dice::SetOverPlane() {
	Float3 underaxis(0.0f, -1.0f, 0.0f);
	Float3 axis;
	bool sts[6];
	float range = 0.001f;
	int diceNum[6] = { 4,6,2,3,1,5 };
	float dot;

	// X軸取り出し
	axis = { mTransform.worldMtx._11, mTransform.worldMtx._12, mTransform.worldMtx._13 };
	// X軸ベクトルと下向きベクトルの内積を計算
	DX11Vec3Dot(dot, axis, underaxis);

	// 一定の幅を持たせて値の比較を行う
	sts[0] = floatcheck(dot, 1.0f, range);
	// Y軸取得
	axis = { mTransform.worldMtx._21, mTransform.worldMtx._22, mTransform.worldMtx._23 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[1] = floatcheck(dot, 1.0f, range);
	// Z軸取得
	axis = { mTransform.worldMtx._31, mTransform.worldMtx._32, mTransform.worldMtx._33 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[2] = floatcheck(dot, 1.0f, range);
	// -X軸取得
	axis = { -mTransform.worldMtx._11, -mTransform.worldMtx._12, -mTransform.worldMtx._13 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[3] = floatcheck(dot, 1.0f, range);
	// -Y軸取得
	axis = { -mTransform.worldMtx._21, -mTransform.worldMtx._22, -mTransform.worldMtx._23 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[4] = floatcheck(dot, 1.0f, range);
	// -Z軸取得
	axis = { -mTransform.worldMtx._31, -mTransform.worldMtx._32, -mTransform.worldMtx._33 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[5] = floatcheck(dot, 1.0f, range);

	for (int i = 0; i < 6; i++)
	{
		if (sts[i])
		{
			mTopDiceTypeFruit = static_cast<DICEFRUIT>(i);
			mTopDiceTypeNum = diceNum[i];
			break;
		}
	}
}

void Dice::OnCollisionEnter(ComponentBase* _oher)
{
}

void Dice::OnCollisionStay(ComponentBase* _oher)
{
}

void Dice::OnCollisionExit(ComponentBase* _oher)
{
}