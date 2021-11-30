#include	"dice.h"
//#include	"../../system/util/XAudio2.h"

Dice::~Dice()
{
	//Finalize();
}

void Dice::Init()
{
	//単位行列化
	DX11MtxIdentity(mTransform.scaleMtx);
	DX11MtxScale(7.0f, 7.0f, 7.0f, mTransform.scaleMtx);
	mTransform.mtx = mTransform.localMtx = mTransform.scaleMtx;
	mTransform.CreateMtx();
	mTransform.CreateScaleMtx();
	// 初期値を入れる
	mTransform.angle = { 0,0,0 };
	mDirection = DIRECTION::NEUTRAL;
	mCrrentRotCnt = 0;
}

void Dice::ObjectUpdate()
{
	//ニュートラル以外で落ちていない場合
	if (mDirection != DIRECTION::NEUTRAL)
	{
		//行列を作成(ワールドの軸を中心に回転)
		DX11MtxMultiply(mTransform.mtx, mTransform.mtx, mMtxFrame);

		//半径を計算
		const static float radius = static_cast<float>((DICESCALE / 2.0f)*sqrt(2));		// DICESCALE*ルート2/2＝DICESCALE/2.0f*ルート2

		//45度から回転角度を足し算
		float nowcenterposy = radius * sin(ToRad(45 + mRotAnglePerFrame * mCrrentRotCnt));

		//移動量の計算
		Float3 pos = { mStartPos.x,0.0f,mStartPos.z };
		Float3 endpos;
		float t;

		if (mDirection == DIRECTION::RIGHT)
		{
			//割合を計算
			t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mRotCnt);
			//終了位置を計算
			endpos.x = mStartPos.x + DICESCALE;
			//線形補間の式でX座標を計算
			pos.x = mStartPos.x *(1.0f - t) + endpos.x*t;
		}

		if (mDirection == DIRECTION::LEFT)
		{
			//割合を計算
			t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mRotCnt);
			//終了位置を計算
			endpos.x = mStartPos.x - DICESCALE;
			//線形補間の式でX座標を計算
			pos.x = mStartPos.x *(1.0f - t) + endpos.x*t;
		}

		if (mDirection == DIRECTION::UP)
		{
			//割合を計算
			t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mRotCnt);
			//終了位置を計算
			endpos.z = mStartPos.z + DICESCALE;
			//線形補間の式でX座標を計算
			pos.z = mStartPos.z *(1.0f - t) + endpos.z*t;
		}

		if (mDirection == DIRECTION::DOWN)
		{
			//割合を計算
			t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mRotCnt);
			//終了位置を計算
			endpos.z = mStartPos.z - DICESCALE;
			//線形補間の式でX座標を計算
			pos.z = mStartPos.z *(1.0f - t) + endpos.z*t;
		}

		//原点の位置を補正
		mTransform.mtx._41 = pos.x;
		mTransform.mtx._42 = nowcenterposy - DICESCALE / 2.0f;
		mTransform.mtx._43 = pos.z;

		//回転数をカウントアップ
		mCrrentRotCnt++;

		// 回転が終わったら元の状態に戻す
		if (mCrrentRotCnt >= mRotCnt)
		{
			//PlaySound(SOUND_LABEL_SE_DICE);
			mDirection = DIRECTION::NEUTRAL;
			mCrrentRotCnt = 0;
			// 回転後の面に設定
			mTopDiceType = OverPlane();
			/// 接しているブロックと面が同じかチェック
			/// ステップ数減らす
		}
	}
}

void Dice::ObjectDraw()
{
	// モデル描画
	//mpModel->Draw(mTransform.mtx);
}

void Dice::Uninit()
{
	//mpModel->Uninit();
	//mpDownModel->Uninit();
}

void Dice::MoveDiceScale(DIRECTION _direction)
{
	switch (_direction)
	{
	case DIRECTION::UP:
		mTransform.mtx._43 += DICESCALE;
		break;
	case DIRECTION::DOWN:
		mTransform.mtx._43 -= DICESCALE;
		break;
	case DIRECTION::LEFT:
		mTransform.mtx._41 -= DICESCALE;
		break;
	case DIRECTION::RIGHT:
		mTransform.mtx._41 += DICESCALE;
		break;
	}
}

void Dice::OnCollisionEnter(GameObject* _oher)
{
}

void Dice::OnCollisionStay(GameObject* _oher)
{
}

void Dice::OnCollisionExit(GameObject* _oher)
{
}

bool Dice::Push(DIRECTION _direction)
{
	return false;
}

bool Dice::Roll(DIRECTION _direction)
{
	//if (// 回転不可なら)
	//{
	//return false;
	//}

	//else(// 回転可能なら)｛

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
	DX11MtxMultiply(mTransform.mtx, mTransform.mtx, mTransform.localMtx);
	mTopDiceType = OverPlane();
	//}
	return true;
}

void Dice::SetRollDirection(DIRECTION _direction)
{
	// 平均移動量、角度を初期化
	Float3 trans, angle;

	//ニュートラルの時だけキー入力を認める
	if (mDirection == DIRECTION::NEUTRAL)
	{
		switch (_direction)
		{
		case DIRECTION::UP:
			mDirection = DIRECTION::UP;
			mTransform.angle.x = mRotAnglePerFrame;
			break;
		case DIRECTION::DOWN:
			mDirection = DIRECTION::DOWN;
			mTransform.angle.x = -mRotAnglePerFrame;
			break;
		case DIRECTION::LEFT:
			mDirection = DIRECTION::LEFT;
			mTransform.angle.z = mRotAnglePerFrame;
			break;
		case DIRECTION::RIGHT:
			mDirection = DIRECTION::RIGHT;
			mTransform.angle.z = -mRotAnglePerFrame;
			break;
		}
		mCrrentRotCnt = 0;
		DX11MakeWorldMatrix(mMtxFrame, mTransform.angle, trans);
		//開始位置を保存
		mStartPos = { mTransform.mtx._41, mTransform.mtx._42,mTransform.mtx._43 };
	}
}

//増した方向の面を特定
DICETYPE Dice::OverPlane() {
	Float3 underaxis = { 0,-1,0 };
	Float3 axis;
	bool sts[6];
	float dot;

	// X軸取り出し
	axis = { mTransform.mtx._11, mTransform.mtx._12, mTransform.mtx._13 };
	// X軸ベクトルと下向きベクトルの内積を計算
	DX11Vec3Dot(dot, axis, underaxis);

	// 一定の幅を持たせて値の比較を行う
	sts[0] = floatcheck(dot, 7.0f, 0.001f);

	// Y軸取得
	axis = { mTransform.mtx._21, mTransform.mtx._22, mTransform.mtx._23 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[1] = floatcheck(dot, 7.0f, 0.001f);
	// Z軸取得
	axis = { mTransform.mtx._31, mTransform.mtx._32, mTransform.mtx._33 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[2] = floatcheck(dot, 7.0f, 0.001f);
	// -X軸取得
	axis = { -mTransform.mtx._11, -mTransform.mtx._12, -mTransform.mtx._13 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[3] = floatcheck(dot, 7.0f, 0.001f);
	// -Y軸取得
	axis = { -mTransform.mtx._21, -mTransform.mtx._22, -mTransform.mtx._23 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[4] = floatcheck(dot, 7.0f, 0.001f);
	// -Z軸取得
	axis = { -mTransform.mtx._31, -mTransform.mtx._32, -mTransform.mtx._33 };
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