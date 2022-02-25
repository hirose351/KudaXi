#include	"dice.h"
#include	"effect_thunder.h"
#include	"effect_fruit.h"
#include	"../manager/dice_manager.h"
#include	"../manager/scene_manager.h"
#include	"../component/model_component.h"
#include	"../component/collision_component.h"
#include	"../../system/util/XAudio2.h"
#include	"../../system/util/easing.h"
#include	"../../system/model/model_manager.h"

Dice::Dice() :GameObject(("Dice"), ObjectType::eDice, true)
{
	bool sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/dice/Dice.fbx",
		"shader/vs.hlsl", "shader/toonps.hlsl",
		"assets/model/dice/");
	if (!sts)
	{
		MessageBox(nullptr, "Diceモデル 読み込みエラー", "error", MB_OK);
	}
	AddComponent<Component::Model>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/dice/Dice.fbx"));
	AddComponent<Component::Collision>()->SetInitState(ObjectTag::eDice, Float3(0, 0, 0), Float3(DICE_SCALE_HALF), DirectX::XMFLOAT4(1, 1, 1, 0.0f));
	GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 1, 1, 0));
	GetComponent<Component::Collision>()->SetOrderInLayer(75);
	GetComponent<Component::Model>()->SetOrderInLayer(70);
}

Dice::~Dice()
{
}

void Dice::ObjectInit()
{
	if (SceneManager::GetInstance()->GetGameMode() != GameMode::eEndless)
		mDiceSts = DiceStatus::eCreate;

	if (mDiceSts == DiceStatus::eCreate)
		return;

	mDirection = Direction::eNeutral;
	SetStartUpPosition();
	SetOverPlane();
	Dix::sp<Effect::Thunder> effect;
	effect.SetPtr(new Effect::Thunder);
	SceneManager::GetInstance()->GetScene(mSceneKey)->AddGameObject(effect);

	Float3 pos = { mTransform->position.x , 0, mTransform->position.z - DICE_SCALE_HALF };
	effect->SetInitPos(pos);
	effect->Init();
}

void Dice::ObjectUpdate()
{
	switch (mDiceSts)
	{
	case DiceStatus::eUp:
		GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 1, 1, 0));
		Up();
		break;
	case DiceStatus::eHalfUp:
		GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 1, 1, 0.5f));
		Up();
		break;
	case DiceStatus::eRoll:
		GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 1, 1, 0));
		Roll();
		break;
	case DiceStatus::ePush:
		GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 1, 1, 0));
		Push();
		break;
	case DiceStatus::eDown:
		//GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 0, 0, 0.5f));
		Down();
		break;
	case DiceStatus::eHalfDown:
		GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 0, 0, 0.3f));
		Down();
		break;
	case DiceStatus::eNormal:
		GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 1, 1, 0));
		break;
	}
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

bool Dice::SetPushAction(Direction _direction)
{
	if (mDiceSts != DiceStatus::eNormal)
		return false;
	if (!DiceManager::GetInstance()->CanDiceMove(this, _direction))
		return false;

	mTransform->move = 0;
	switch (_direction)
	{
	case Direction::eUp:
		mTransform->move.z = mPushPositionPerFrame;
		break;
	case Direction::eDown:
		mTransform->move.z = -mPushPositionPerFrame;
		break;
	case Direction::eLeft:
		mTransform->move.x = -mPushPositionPerFrame;
		break;
	case Direction::eRight:
		mTransform->move.x = mPushPositionPerFrame;
		break;
	}
	mDiceSts = DiceStatus::ePush;
	mDirection = _direction;
	mCrrentPushCnt = 0;
	return true;
}

bool Dice::SetRollAction(Direction _direction)
{
	if (mDiceSts != DiceStatus::eNormal)
		return false;
	if (!DiceManager::GetInstance()->CanDiceMove(this, _direction))
		return false;
	mTransform->angle = mBeforeFramePos = mBeforeFrameAng = 0;
	switch (_direction)
	{
	case Direction::eUp:
		mTransform->angle.x = 90.0f;
		break;
	case Direction::eDown:
		mTransform->angle.x = -90.0f;
		break;
	case Direction::eLeft:
		mTransform->angle.z = 90.0f;
		break;
	case Direction::eRight:
		mTransform->angle.z = -90.0f;
		break;
	}
	mDiceSts = DiceStatus::eRoll;
	mDirection = _direction;
	mCrrentRotCnt = 0;
	mStartMtx = mTransform->worldMtx;
	DX11MakeWorldMatrix(mTargetMtx, mTransform->angle, XMFLOAT3(0, 0, 0));
	// 行列を作成(ワールドの軸を中心に回転)
	DX11MtxMultiply(mTargetMtx, mTransform->worldMtx, mTargetMtx);
	mRotateStartPos = { mTransform->worldMtx._41, mTransform->worldMtx._42,mTransform->worldMtx._43 };
	return true;
}

bool Dice::CheckDiceDirection(Direction _direction)
{
	return DiceManager::GetInstance()->CanDiceMoveCheak(this, _direction);
}

void Dice::SetStartUpPosition()
{
	mTransform->move.y = mUpPositionPerFrame;
	mDiceSts = DiceStatus::eHalfUp;
}

void Dice::SetDownPosition()
{
	mChainCnt++;	// チェイン数加算

	if (mDiceSts != DiceStatus::eDown && mDiceSts != DiceStatus::eHalfDown)
	{
		mTransform->move = Float3(0, -mDownPositionPerFrame, 0);
		mDiceSts = DiceStatus::eDown;
		GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 0, 0, 0.5f));
	}
	else
	{
		mTransform->position.y += 2.0f;
		if (mTopDiceTypeNum == 1)
			return;
	}


	// エフェクト生成
	Dix::sp<Effect::Fruit> effect;
	effect.SetPtr(new Effect::Fruit);
	effect->SetInit(Float3(mTransform->worldMtx._41, 50, mTransform->worldMtx._43), (int)mTopDiceTypeFruit);
	SceneManager::GetInstance()->GetScene(mSceneKey)->AddGameObject(effect);
}

void Dice::Push()
{
	mTransform->MovePosition();
	mCrrentPushCnt++;
	if (mCrrentPushCnt >= mMoveCnt)
	{
		mDirection = Direction::eNeutral;
		mDiceMoveSts = mDiceSts;
		mDiceSts = DiceStatus::eNormal;
		// 接しているブロックと面が同じかチェック
		DiceManager::GetInstance()->CheckAligned(this);
		PlaySound(SOUND_LABEL_SE_DICE);
	}
}

void Dice::Roll()
{
	// 半径を計算
	const static float radius = static_cast<float>(DICE_SCALE_HALF*sqrt(2));		// DICESCALE*ルート2/2＝DICESCALE/2.0f*ルート2

	// 割合を計算
	float frameAngle = EasingProcess::CircOut(static_cast<float>(mCrrentRotCnt), static_cast<float>(mMoveCnt + 1), 0.0f, 90.0f);
	float framePos = EasingProcess::CircOut(static_cast<float>(mCrrentRotCnt), static_cast<float>(mMoveCnt + 1), 0.0f, DICE_SCALE);

	// 45度から回転角度を足し算
	float nowcenterposy = radius * sin(ToRad(45 + frameAngle));
	// 移動量の計算
	Float3 pos = { mRotateStartPos.x,nowcenterposy,mRotateStartPos.z };
	Float3 endpos = pos;

	switch (mDirection)
	{
	case Direction::eUp:
		mTransform->angle.x = frameAngle;
		//  終了位置を計算
		endpos.z = mRotateStartPos.z + DICE_SCALE;
		pos.z = mRotateStartPos.z + framePos;
		break;
	case Direction::eDown:
		mTransform->angle.x = -frameAngle;
		endpos.z = mRotateStartPos.z - DICE_SCALE;
		pos.z = mRotateStartPos.z - framePos;
		break;
	case Direction::eLeft:
		mTransform->angle.z = frameAngle;
		endpos.x = mRotateStartPos.x - DICE_SCALE;
		pos.x = mRotateStartPos.x - framePos;
		break;
	case Direction::eRight:
		mTransform->angle.z = -frameAngle;
		endpos.x = mRotateStartPos.x + DICE_SCALE;
		pos.x = mRotateStartPos.x + framePos;
		break;
	}

	mBeforeFramePos = framePos;
	mBeforeFrameAng = frameAngle;

	XMFLOAT4X4 rotatinMtx;

	DX11MakeWorldMatrix(rotatinMtx, mTransform->angle, XMFLOAT3(0, 0, 0));
	// 行列を作成(ワールドの軸を中心に回転)
	DX11MtxMultiply(mTransform->worldMtx, mStartMtx, rotatinMtx);
	// 原点の位置を補正
	mTransform->SetPositionXYZ(pos);

	// 回転が終わったら元の状態に戻す
	if (mCrrentRotCnt >= mMoveCnt)
	{
		mTransform->worldMtx = mTargetMtx;
		endpos.y = DICE_SCALE_HALF;
		mTransform->SetPositionXYZ(endpos);
		mDirection = Direction::eNeutral;
		mDiceMoveSts = mDiceSts;
		mDiceSts = DiceStatus::eNormal;
		// 回転後の面に設定
		SetOverPlane();
		// 接しているブロックと面が同じかチェック
		DiceManager::GetInstance()->CheckAligned(this);
		PlaySound(SOUND_LABEL_SE_DICE);
	}
	// 回転数をカウントアップ
	mCrrentRotCnt++;
}

void Dice::Up()
{
	mTransform->MovePosition();
	mCrrentPushCnt++;
	if (mCrrentPushCnt >= mUpCnt)
	{
		mDirection = Direction::eNeutral;
		mDiceSts = DiceStatus::eNormal;
	}
	else if (mCrrentPushCnt >= mUpCnt / 2)
	{
		mDirection = Direction::eNeutral;
		mDiceSts = DiceStatus::eUp;
	}
}

void Dice::Down()
{
	mTransform->MovePosition();
	if (mTransform->position.y < -DICE_SCALE_HALF)
	{
		DiceManager::GetInstance()->SetRemoveDice(GetObjectID());
	}
	else if (mTransform->position.y < 0)
	{
		mDirection = Direction::eNeutral;
		mDiceSts = DiceStatus::eHalfDown;
	}
}

void Dice::SetOverPlane() {
	Float3 underaxis(0.0f, -1.0f, 0.0f);
	Float3 axis;
	bool sts[6];
	float range = 0.001f;
	int diceNum[6] = { 4,6,2,3,1,5 };
	float dot;

	// X軸取り出し
	axis = { mTransform->worldMtx._11, mTransform->worldMtx._12, mTransform->worldMtx._13 };
	// X軸ベクトルと下向きベクトルの内積を計算
	DX11Vec3Dot(dot, axis, underaxis);

	// 一定の幅を持たせて値の比較を行う
	sts[0] = floatcheck(dot, 1.0f, range);
	// Y軸取得
	axis = { mTransform->worldMtx._21, mTransform->worldMtx._22, mTransform->worldMtx._23 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[1] = floatcheck(dot, 1.0f, range);
	// Z軸取得
	axis = { mTransform->worldMtx._31, mTransform->worldMtx._32, mTransform->worldMtx._33 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[2] = floatcheck(dot, 1.0f, range);
	// -X軸取得
	axis = { -mTransform->worldMtx._11, -mTransform->worldMtx._12, -mTransform->worldMtx._13 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[3] = floatcheck(dot, 1.0f, range);
	// -Y軸取得
	axis = { -mTransform->worldMtx._21, -mTransform->worldMtx._22, -mTransform->worldMtx._23 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[4] = floatcheck(dot, 1.0f, range);
	// -Z軸取得
	axis = { -mTransform->worldMtx._31, -mTransform->worldMtx._32, -mTransform->worldMtx._33 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[5] = floatcheck(dot, 1.0f, range);

	for (int i = 0; i < 6; i++)
	{
		if (sts[i])
		{
			mTopDiceTypeFruit = static_cast<DiceFruit>(i);
			mTopDiceTypeNum = diceNum[i];
			break;
		}
	}
}