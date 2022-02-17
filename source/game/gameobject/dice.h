#pragma once
#include	"gameobject.h"
#include	"../state/dice_state.h"
#include	"../../system/util/CBillboard.h"

using Microsoft::WRL::ComPtr;

class Dice : public GameObject
{
private:
	float mBeforeFrameAng = 0;										// 
	float mBeforeFramePos = 0;										// 
	INT3 mMapPos;													// マップ上の位置
	DiceFruit mTopDiceTypeFruit;									// 上面の果物
	int mTopDiceTypeNum;											// 上面の数字
	DiceStatus mDiceSts = DiceStatus::eNormal;						// 状態
	DiceStatus mDiceMoveSts = DiceStatus::eNormal;					// 移動時の状態
	Direction mDirection = Direction::eNeutral;						// サイコロの移動方向	

	DirectX::XMFLOAT4X4 mStartMtx;									// 1フレームでの変化を表す行列	
	DirectX::XMFLOAT4X4 mTargetMtx;									// 1回転での変化を表す行列
	Float3 mRotateStartPos;										// キー入力された際の開始位置	

	const int mMoveCnt = 16;										// 90度回転、押されるのに必要な更新回数
	int mCrrentRotCnt = 0;											// 今の回転回数	

	int mCrrentPushCnt = 0;										// 今の回転回数
	const float mPushPositionPerFrame = DICE_SCALE / mMoveCnt;		// 

	const int mUpDownCnt = 200;									// 
	const float mUpDownPositionPerFrame = DICE_SCALE / mUpDownCnt;		// 

	// 移動する
	void Push();
	// 回転する
	void Roll();
	// 生成時上がる
	void Up();
	// 揃った時下がる
	void Down();

public:
	Dice();
	virtual ~Dice();

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectImguiDraw()override {};
	void Uninit()override;

	// 初期面セット
	void SetInitType() {
		SetOverPlane();
	}

	void SetStartUpPosition();
	void SetDownPosition();

	// 上の面を特定
	void SetOverPlane();

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
		return mDiceSts;
	}

	// モデルの状態を取得する(沈んでいるか)
	DiceStatus GetDiceMoveStatus() {
		return mDiceMoveSts;
	}

	Direction GetDirection() {
		return mDirection;
	}

	// ハッピーワン呼び出し
	void SetHappyOne();

	void SetMapPos(INT3 _i3) { mMapPos = _i3; };
	INT3 GetMapPos() { return mMapPos; };

	bool GetPushEnd() { return mCrrentPushCnt == mMoveCnt; }
	bool GetRollEnd() { return mCrrentRotCnt >= mMoveCnt; }

	void SetDiceSts(DiceStatus _sts) { mDiceSts = _sts; }
};

