#pragma once
#include	"gameobject.h"
#include	"../../system/model/ModelMgr.h"
#include	"../component/allcomponents.h"
#include	"../state/dice_state.h"
#include	"../../system/util/CBillboard.h"
#include	"../../system/util/StopWatch.h"

using Microsoft::WRL::ComPtr;

class Dice : public GameObject
{
private:
	float ang = 0;
	float mBeforeFramePos = 0;							// 
	float mThunderAlha = 1.0f;							// 雷α値
	CBillboard mThunder;								// 生成時雷
	INT3 mMapPos;										// マップ上の位置
	DiceFruit mTopDiceTypeFruit;						// 上面の果物
	int mTopDiceTypeNum;								// 上面の数字
	DiceStatus mSts;									// 状態
	Direction mDirection = Direction::eNeutral;			// サイコロの移動方向	

	DirectX::XMFLOAT4X4 mFrameMtx;						// 1フレームでの変化を表す行列	
	DirectX::XMFLOAT4X4 mTargetMtx;						// 1回転での変化を表す行列
	Float3 mRotateStartPos;								// キー入力された際の開始位置	

	const int mMoveCnt = 15;							// 90度回転、押されるのに必要な更新回数
	int mCrrentRotCnt = 0;								// 今の回転回数	
	const float mRotAnglePerFrame = 90.0f / mMoveCnt;	// 1回当たりの回転角度	

	int mCrrentPushCnt = 0;								// 今の回転回数
	const float mPushPositionPerFrame = DICE_SCALE / mMoveCnt;

	const int mUpCnt = 150;
	const float mUpPositionPerFrame = DICE_SCALE / mUpCnt;

	// 上の面を特定
	void SetOverPlane();
	// 回転移動セット
	void SetRollDirection(Direction _direction);

	// 移動する
	void Push();
	// 回転する
	void Roll();
	// 生成時上がる
	void Up();
	// 揃った時下がる
	void Down();

public:
	Dice() :GameObject(("Dice"), ObjectType::eDice) {
		bool sts = ModelMgr::GetInstance().LoadModel(
			"assets/model/dice/Dice.fbx",
			"shader/vs.hlsl", "shader/pstexcol.hlsl",
			"assets/model/dice/");
		if (!sts)
		{
			MessageBox(nullptr, "Diceモデル 読み込みエラー", "error", MB_OK);
		}
		AddComponent<Component::ModelComponent>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/dice/Dice.fbx"));
		AddComponent<Component::CollisionComponent>()->SetInitState(ObjectTag::eDice, Float3(0, 0, 0), Float3(DICE_SCALE_HALF), DirectX::XMFLOAT4(0, 0, 1, 0.3f));
		ObjectInit();
	}
	~Dice();

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectDraw()override;
	void ObjectImguiDraw()override {};
	void Uninit()override;

	// 初期面セット
	void SetInitType() {
		SetOverPlane();
	}

	void SetStartUpPosition();
	void SetDownPosition();

	// 指定方向に移動
	bool SetPushAction(Direction _direction);
	// 指定方向に回転
	bool SetRollAction(Direction _direction);
	// 指定方向にサイコロがあるか
	bool CheckDiceDirection(Direction _direction);

	// 上面取得
	DiceFruit GetTopDiceType() {
		return mTopDiceTypeFruit;
	}
	// 上面取得
	int GetTopDiceTypeNum() {
		return mTopDiceTypeNum;
	}

	// モデルの状態を取得する(沈んでいるか)
	DiceStatus GetDiceStatus() {
		return mSts;
	}

	Direction GetDirection() {
		return mDirection;
	}

	// ハッピーワン呼び出し
	void SetHappyOne();

	// 1マス分移動
	void MoveDiceScale(Direction _direction);

	void SetMapPos(INT3 _i3) { mMapPos = _i3; };
	INT3 GetMapPos() { return mMapPos; };

	void OnCollisionEnter(ComponentBase* _oher) override;
	void OnCollisionStay(ComponentBase* _oher) override;
	void OnCollisionExit(ComponentBase* _oher) override;

	float GetmPushPositionPerFrame() { return mPushPositionPerFrame; }
	float GetmRollmRotAnglePerFrame() { return mRotAnglePerFrame; }
	bool GetPushEnd() { return mCrrentPushCnt == mMoveCnt; }
	bool GetRollEnd() { return mCrrentRotCnt >= mMoveCnt; }
};

