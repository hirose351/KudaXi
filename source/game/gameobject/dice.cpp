#include	"dice.h"
#include	"../manager/dice_manager.h"
//#include	"../../system/util/XAudio2.h"

Dice::~Dice()
{
	//Finalize();
}

void Dice::ObjectInit()
{
	//単位行列化
	//DX11MtxIdentity(mTransform.scaleMtx);
	//DX11MtxScale(7.0f, 7.0f, 7.0f, mTransform.scaleMtx);
	mTransform.worldMtx = mTransform.localMtx = mTransform.scaleMtx;
	mTransform.CreateMtx();
	//mTransform.CreateScaleMtx();
	// 初期値を入れる
	mTransform.angle = { 0,0,0 };
	mDirection = DIRECTION::NEUTRAL;
	mSts = DICESTATUS::NORMAL;
	Up();
}

void Dice::ObjectUpdate()
{
	////ニュートラル以外で落ちていない場合
	//if (mDirection != DIRECTION::NEUTRAL)
	//{
	if (mSts == DICESTATUS::ROLL)
	{
		//行列を作成(ワールドの軸を中心に回転)
		DX11MtxMultiply(mTransform.worldMtx, mTransform.worldMtx, mMtxFrame);

		//半径を計算
		const static float radius = static_cast<float>((DICESCALE / 2.0f)*sqrt(2));		// DICESCALE*ルート2/2＝DICESCALE/2.0f*ルート2

		//45度から回転角度を足し算
		float nowcenterposy = radius * sin(ToRad(45 + mRotAnglePerFrame * mCrrentRotCnt));

		//移動量の計算
		Float3 pos = { mRotateStartPos.x,0.0f,mRotateStartPos.z };
		Float3 endpos;
		float t;

		if (mDirection == DIRECTION::RIGHT)
		{
			//割合を計算
			t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mRotCnt);
			//終了位置を計算
			endpos.x = mRotateStartPos.x + DICESCALE;
			//線形補間の式でX座標を計算
			pos.x = mRotateStartPos.x *(1.0f - t) + endpos.x*t;
		}

		if (mDirection == DIRECTION::LEFT)
		{
			//割合を計算
			t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mRotCnt);
			//終了位置を計算
			endpos.x = mRotateStartPos.x - DICESCALE;
			//線形補間の式でX座標を計算
			pos.x = mRotateStartPos.x *(1.0f - t) + endpos.x*t;
		}

		if (mDirection == DIRECTION::UP)
		{
			//割合を計算
			t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mRotCnt);
			//終了位置を計算
			endpos.z = mRotateStartPos.z + DICESCALE;
			//線形補間の式でX座標を計算
			pos.z = mRotateStartPos.z *(1.0f - t) + endpos.z*t;
		}

		if (mDirection == DIRECTION::DOWN)
		{
			//割合を計算
			t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mRotCnt);
			//終了位置を計算
			endpos.z = mRotateStartPos.z - DICESCALE;
			//線形補間の式でX座標を計算
			pos.z = mRotateStartPos.z *(1.0f - t) + endpos.z*t;
		}

		//原点の位置を補正
		mTransform.worldMtx._41 = pos.x;
		mTransform.worldMtx._42 = nowcenterposy - DICESCALE / 2.0f;
		mTransform.worldMtx._43 = pos.z;

		//回転数をカウントアップ
		mCrrentRotCnt++;

		// 回転が終わったら元の状態に戻す
		if (mCrrentRotCnt >= mRotCnt)
		{
			//PlaySound(SOUND_LABEL_SE_DICE);
			mDirection = DIRECTION::NEUTRAL;
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
		if (mCrrentPushCnt >= mRotCnt)
		{
			mDirection = DIRECTION::NEUTRAL;
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
			mDirection = DIRECTION::NEUTRAL;
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

void Dice::MoveDiceScale(DIRECTION _direction)
{
	switch (_direction)
	{
	case DIRECTION::UP:
		mTransform.worldMtx._43 += DICESCALE;
		break;
	case DIRECTION::DOWN:
		mTransform.worldMtx._43 -= DICESCALE;
		break;
	case DIRECTION::LEFT:
		mTransform.worldMtx._41 -= DICESCALE;
		break;
	case DIRECTION::RIGHT:
		mTransform.worldMtx._41 += DICESCALE;
		break;
	}
}


bool Dice::Push(DIRECTION _direction)
{
	if (!DiceManager::GetInstance()->CanPush(this, _direction))
		return false;

	//ニュートラルの時だけキー入力を認める
	if (mDirection == DIRECTION::NEUTRAL)
	{
		mTransform.move = 0;
		switch (_direction)
		{
		case DIRECTION::UP:
			mTransform.move.z = mPushPisitionPerFrame;
			break;
		case DIRECTION::DOWN:
			mTransform.move.z = -mPushPisitionPerFrame;
			break;
		case DIRECTION::LEFT:
			mTransform.move.x = -mPushPisitionPerFrame;
			break;
		case DIRECTION::RIGHT:
			mTransform.move.x = mPushPisitionPerFrame;
			break;
		}
		mSts = DICESTATUS::PUSH;
		mDirection = _direction;
		mCrrentPushCnt = 0;
	}
	return true;
}

bool Dice::Roll(DIRECTION _direction)
{
	if (!DiceManager::GetInstance()->CanPush(this, _direction))
		return false;

	// 平均移動量、角度
	Float3 trans, angle;

	switch (_direction)
	{
	case DIRECTION::UP:
		angle.x = 90;
		break;
	case DIRECTION::DOWN:
		angle.x = -90;
		break;
	case DIRECTION::LEFT:
		angle.z = 90;
		break;
	case DIRECTION::RIGHT:
		angle.z = -90;
		break;
	}
	DX11MakeWorldMatrix(mTransform.localMtx, angle, trans);
	DX11MtxMultiply(mTransform.worldMtx, mTransform.worldMtx, mTransform.localMtx);
	mTopDiceType = OverPlane();

	return true;
}

void Dice::SetRollDirection(DIRECTION _direction)
{
	//ニュートラルの時だけキー入力を認める
	if (mDirection == DIRECTION::NEUTRAL)
	{
		mDirection = _direction;
		switch (_direction)
		{
		case DIRECTION::UP:
			mTransform.angle.x = mRotAnglePerFrame;
			break;
		case DIRECTION::DOWN:
			mTransform.angle.x = -mRotAnglePerFrame;
			break;
		case DIRECTION::LEFT:
			mTransform.angle.z = mRotAnglePerFrame;
			break;
		case DIRECTION::RIGHT:
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
	Float3 underaxis = { 0,-1,0 };
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