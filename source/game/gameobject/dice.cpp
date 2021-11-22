#include	"dice.h"
#include	"../../system/model/ModelMgr.h"
//#include	"../../system/util/XAudio2.h"

Dice::Dice()
{
	bool sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/dice/Dice.fbx",			// ファイル名 
		"shader/vs.hlsl",						// 頂点シェーダー
		"shader/pstexcol.hlsl",					// ピクセルシェーダー
		"assets/model/dice/");					// テクスチャの格納フォルダ

	if (!sts)
	{
		MessageBox(nullptr, "Diceモデル 読み込みエラー", "error", MB_OK);
	}
	SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/dice/Dice.fbx"));
	Init();
}

Dice::~Dice()
{
	//Finalize();
}

void Dice::Init()
{
	//単位行列化
	DX11MtxIdentity(mMtx);
	DX11MtxIdentity(mLocalMtx);



	DirectX::XMFLOAT4X4 scaleMtx;
	DX11MtxScale(7.0f, 7.0f, 7.0f, scaleMtx);
	DX11MtxMultiply(mMtx, scaleMtx, mMtx);


	DX11MtxIdentity(mMtxFrame);
	DX11MtxMultiply(mMtxFrame, scaleMtx, mMtxFrame);
	// 初期値を入れる
	mAngle = { 0,0,0 };
	DIRECTION m_direction = DIRECTION::NEUTRAL;

	mCrrentRotCnt = 0;
	DX11MtxMultiply(mLocalMtx, scaleMtx, mLocalMtx);
}

void Dice::Update()
{
	//ニュートラル以外で落ちていない場合
	if (mDirection != DIRECTION::NEUTRAL)
	{
		//行列を作成(ワールドの軸を中心に回転)
		DX11MtxMultiply(mMtx, mMtx, mMtxFrame);

		//半径を計算
		const static float radius = static_cast<float>(7.5f*sqrt(2));		//15×ルート2÷2＝7.5f×ルート2

		//45度から回転角度を足し算
		float nowcenterposy = radius * sin(ToRad(45 + mRotAnglePerFrame * mCrrentRotCnt));

		//移動量の計算
		Vector3 pos = { mStartPos.x,0.0f,mStartPos.z };
		Vector3 endpos;
		float t;

		if (mDirection == DIRECTION::RIGHT)
		{
			//割合を計算
			t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mRotCnt);
			//終了位置を計算
			endpos.x = mStartPos.x + 15.0f;
			//線形補間の式でX座標を計算
			pos.x = mStartPos.x *(1.0f - t) + endpos.x*t;
		}

		if (mDirection == DIRECTION::LEFT)
		{
			//割合を計算
			t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mRotCnt);
			//終了位置を計算
			endpos.x = mStartPos.x - 15.0f;
			//線形補間の式でX座標を計算
			pos.x = mStartPos.x *(1.0f - t) + endpos.x*t;
		}

		if (mDirection == DIRECTION::UP)
		{
			//割合を計算
			t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mRotCnt);
			//終了位置を計算
			endpos.z = mStartPos.z + 15.0f;
			//線形補間の式でX座標を計算
			pos.z = mStartPos.z *(1.0f - t) + endpos.z*t;
		}

		if (mDirection == DIRECTION::DOWN)
		{
			//割合を計算
			t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mRotCnt);
			//終了位置を計算
			endpos.z = mStartPos.z - 15.0f;
			//線形補間の式でX座標を計算
			pos.z = mStartPos.z *(1.0f - t) + endpos.z*t;
		}

		//原点の位置を補正
		mMtx._41 = pos.x;
		mMtx._42 = nowcenterposy - DICESCALE / 2.0f;
		mMtx._43 = pos.z;

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
			// 接しているブロックと面が同じかチェック
			// ステップ数減らす
		}
	}
}

void Dice::Draw()
{
	// モデル描画
	mpModel->Draw(mMtx);
}

void Dice::Finalize()
{
	//mpModel->Uninit();
	//mpDownModel->Uninit();
}

void Dice::MoveDiceScale(DIRECTION _direction)
{
	switch (_direction)
	{
	case DIRECTION::UP:
		mMtx._43 += DICESCALE;
		break;
	case DIRECTION::DOWN:
		mMtx._43 -= DICESCALE;
		break;
	case DIRECTION::LEFT:
		mMtx._41 -= DICESCALE;
		break;
	case DIRECTION::RIGHT:
		mMtx._41 += DICESCALE;
		break;
	}
}

void Dice::Rotation(DIRECTION _direction)
{
	// 平均移動量、角度を初期化
	Vector3 trans, angle;

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
	DX11MakeWorldMatrix(mLocalMtx, angle, trans);
	DX11MtxMultiply(mMtx, mMtx, mLocalMtx);
	mTopDiceType = OverPlane();
}

void Dice::SetRollDirection(DIRECTION _direction)
{
	// 平均移動量、角度を初期化
	Vector3 trans, angle;

	//ニュートラルの時だけキー入力を認める
	if (mDirection == DIRECTION::NEUTRAL)
	{
		switch (_direction)
		{
		case DIRECTION::UP:
			mDirection = DIRECTION::UP;
			mAngle.x = mRotAnglePerFrame;
			break;
		case DIRECTION::DOWN:
			mDirection = DIRECTION::DOWN;
			mAngle.x = -mRotAnglePerFrame;
			break;
		case DIRECTION::LEFT:
			mDirection = DIRECTION::LEFT;
			mAngle.z = mRotAnglePerFrame;
			break;
		case DIRECTION::RIGHT:
			mDirection = DIRECTION::RIGHT;
			mAngle.z = -mRotAnglePerFrame;
			break;
		}
		mCrrentRotCnt = 0;
		DX11MakeWorldMatrix(mMtxFrame, mAngle, trans);
		//開始位置を保存
		mStartPos = { mMtx._41, mMtx._42,mMtx._43 };
	}
}

//増した方向の面を特定
DICETYPE Dice::OverPlane() {
	Vector3 underaxis = { 0,-1,0 };
	Vector3 axis;
	bool sts[6];
	float dot;

	// X軸取り出し
	axis = { mMtx._11, mMtx._12, mMtx._13 };
	// X軸ベクトルと下向きベクトルの内積を計算
	DX11Vec3Dot(dot, axis, underaxis);

	// 一定の幅を持たせて値の比較を行う
	sts[0] = floatcheck(dot, 7.0f, 0.001f);

	// Y軸取得
	axis = { mMtx._21, mMtx._22, mMtx._23 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[1] = floatcheck(dot, 7.0f, 0.001f);
	// Z軸取得
	axis = { mMtx._31, mMtx._32, mMtx._33 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[2] = floatcheck(dot, 7.0f, 0.001f);
	// -X軸取得
	axis = { -mMtx._11, -mMtx._12, -mMtx._13 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[3] = floatcheck(dot, 7.0f, 0.001f);
	// -Y軸取得
	axis = { -mMtx._21, -mMtx._22, -mMtx._23 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[4] = floatcheck(dot, 7.0f, 0.001f);
	// -Z軸取得
	axis = { -mMtx._31, -mMtx._32, -mMtx._33 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[5] = floatcheck(dot, 7.0f, 0.001f);

	const char* tbl[] = {
	"PEACH",
	"MELON",
	"BLUEBERRY",
	"BANANA",
	"APPLE",
	"ORANGE",
	};

	for (int i = 0; i < 6; i++)
	{
		if (sts[i])
		{
			return (DICETYPE)i;
			break;
		}
	}
	return DICETYPE::APPLE;
}