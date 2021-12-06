#include	"dice.h"
#include	"../manager/dice_manager.h"
//#include	"../../system/util/XAudio2.h"

Dice::~Dice()
{
	//Finalize();
}

void Dice::ObjectInit()
{
	mDirection = Direction::eNeutral;
	mSts = DICESTATUS::NORMAL;
	Up();
}

void Dice::ObjectUpdate()
{
	if (mSts == DICESTATUS::ROLL)
	{
		//行列を作成(ワールドの軸を中心に回転)
		DX11MtxMultiply(mTransform.worldMtx, mTransform.worldMtx, mMtxFrame);

		//半径を計算
		const static float radius = static_cast<float>((DICE_SCALE / 2.0f)*sqrt(2));		// DICESCALE*ルート2/2＝DICESCALE/2.0f*ルート2

		//45度から回転角度を足し算
		float nowcenterposy = radius * sin(ToRad(45 + mRotAnglePerFrame * mCrrentRotCnt));

		//移動量の計算
		Float3 pos = { mRotateStartPos.x,0.0f,mRotateStartPos.z };
		Float3 endpos;
		//割合を計算
		float t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mMoveCnt);

		if (mDirection == Direction::eRight)
		{
			//終了位置を計算
			endpos.x = mRotateStartPos.x + DICE_SCALE;
			//線形補間の式でX座標を計算
			pos.x = mRotateStartPos.x *(1.0f - t) + endpos.x*t;
		}

		if (mDirection == Direction::eLeft)
		{
			//終了位置を計算
			endpos.x = mRotateStartPos.x - DICE_SCALE;
			//線形補間の式でX座標を計算
			pos.x = mRotateStartPos.x *(1.0f - t) + endpos.x*t;
		}

		if (mDirection == Direction::eUp)
		{
			//終了位置を計算
			endpos.z = mRotateStartPos.z + DICE_SCALE;
			//線形補間の式でX座標を計算
			pos.z = mRotateStartPos.z *(1.0f - t) + endpos.z*t;
		}

		if (mDirection == Direction::eDown)
		{
			//終了位置を計算
			endpos.z = mRotateStartPos.z - DICE_SCALE;
			//線形補間の式でX座標を計算
			pos.z = mRotateStartPos.z *(1.0f - t) + endpos.z*t;
		}

		//原点の位置を補正
		mTransform.worldMtx._41 = pos.x;
		mTransform.worldMtx._42 = nowcenterposy - DICE_SCALE / 2.0f;
		mTransform.worldMtx._43 = pos.z;

		//回転数をカウントアップ
		mCrrentRotCnt++;

		// 回転が終わったら元の状態に戻す
		if (mCrrentRotCnt >= mMoveCnt)
		{
			//PlaySound(SOUND_LABEL_SE_DICE);
			mDirection = Direction::eNeutral;
			mSts = DICESTATUS::NORMAL;
			mCrrentRotCnt = 0;
			// 回転後の面に設定
			mTopDiceType = OverPlane();
			/// 接しているブロックと面が同じかチェック
			/// ステップ数減らす
		}
	}
	else if (mSts == DICESTATUS::PUSH)
	{
		mTransform.AddPosition();
		mTransform.CreateMtx();
		mCrrentPushCnt++;
		if (mCrrentPushCnt >= mMoveCnt)
		{
			mDirection = Direction::eNeutral;
			mSts = DICESTATUS::NORMAL;
		}
	}
	else if (mSts == DICESTATUS::UP)
	{
		mTransform.AddPosition();
		mTransform.CreateMtx();
		mCrrentPushCnt++;
		if (mCrrentPushCnt >= mUpCnt)
		{
			mDirection = Direction::eNeutral;
			mSts = DICESTATUS::NORMAL;
		}
	}
}

void Dice::ObjectDraw()
{
}

void Dice::Uninit()
{
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


bool Dice::Push(Direction _direction)
{
	if (!DiceManager::GetInstance()->CanPush(this, _direction))
		return false;
	if (mDirection != Direction::eNeutral)
		return false;

	mTransform.move = 0;
	switch (_direction)
	{
	case Direction::eUp:
		mTransform.move.z = mPushPisitionPerFrame;
		break;
	case Direction::eDown:
		mTransform.move.z = -mPushPisitionPerFrame;
		break;
	case Direction::eLeft:
		mTransform.move.x = -mPushPisitionPerFrame;
		break;
	case Direction::eRight:
		mTransform.move.x = mPushPisitionPerFrame;
		break;
	}
	mSts = DICESTATUS::PUSH;
	mDirection = _direction;
	mCrrentPushCnt = 0;
	return true;
}

bool Dice::Roll(Direction _direction)
{
	if (!DiceManager::GetInstance()->CanPush(this, _direction))
		return false;

	// 平均移動量、角度
	Float3 trans, angle;

	switch (_direction)
	{
	case Direction::eUp:
		angle.x = 90;
		break;
	case Direction::eDown:
		angle.x = -90;
		break;
	case Direction::eLeft:
		angle.z = 90;
		break;
	case Direction::eRight:
		angle.z = -90;
		break;
	}
	DX11MakeWorldMatrix(mTransform.localMtx, angle, trans);
	DX11MtxMultiply(mTransform.worldMtx, mTransform.worldMtx, mTransform.localMtx);
	mTopDiceType = OverPlane();

	return true;
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

void Dice::Up()
{
	mTransform.move.y = mUpPisitionPerFrame;
	mSts = DICESTATUS::UP;
}

//増した方向の面を特定
DICETYPE Dice::OverPlane() {
	Float3 underaxis(0.0f, -1.0f, 0.0f);
	Float3 axis;
	bool sts[6];
	float dot;

	// X軸取り出し
	axis = { mTransform.worldMtx._11, mTransform.worldMtx._12, mTransform.worldMtx._13 };
	// X軸ベクトルと下向きベクトルの内積を計算
	DX11Vec3Dot(dot, axis, underaxis);

	// 一定の幅を持たせて値の比較を行う
	sts[0] = floatcheck(dot, 7.0f, 0.001f);

	// Y軸取得
	axis = { mTransform.worldMtx._21, mTransform.worldMtx._22, mTransform.worldMtx._23 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[1] = floatcheck(dot, 7.0f, 0.001f);
	// Z軸取得
	axis = { mTransform.worldMtx._31, mTransform.worldMtx._32, mTransform.worldMtx._33 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[2] = floatcheck(dot, 7.0f, 0.001f);
	// -X軸取得
	axis = { -mTransform.worldMtx._11, -mTransform.worldMtx._12, -mTransform.worldMtx._13 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[3] = floatcheck(dot, 7.0f, 0.001f);
	// -Y軸取得
	axis = { -mTransform.worldMtx._21, -mTransform.worldMtx._22, -mTransform.worldMtx._23 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[4] = floatcheck(dot, 7.0f, 0.001f);
	// -Z軸取得
	axis = { -mTransform.worldMtx._31, -mTransform.worldMtx._32, -mTransform.worldMtx._33 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[5] = floatcheck(dot, 7.0f, 0.001f);

	for (int i = 0; i < 6; i++)
	{
		if (sts[i])
		{
			return static_cast<DICETYPE>(i);
			break;
		}
	}
	return DICETYPE::APPLE;
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