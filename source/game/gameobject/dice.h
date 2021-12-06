#pragma once
#include	"gameobject.h"
#include	"../../system/model/ModelMgr.h"
#include	"../component/allcomponents.h"
#include	"../../system/util/INT3.h"

using Microsoft::WRL::ComPtr;

// サイコロの面の種類
enum class DICETYPE {
	PEACH,
	MELON,
	BLUEBERRY,
	BANANA,
	APPLE,
	ORANGE,
	DICETYPE_MAX,
};

// サイコロの状態
enum class DICESTATUS {
	NORMAL,		// 通常
	ROLL,		// 回転
	PUSH,		// 押す
	DOWN,		// 沈む
	HALFDOWN,	// 半分沈む
	UP,	// 半分沈む
};

class Dice : public GameObject
{
private:
	INT3 mMapPos;
	DICETYPE mTopDiceType;								// 上面
	DICESTATUS mSts;	// 生存状態
	Direction mDirection = Direction::eNeutral;			// サイコロの移動方向	

	DirectX::XMFLOAT4X4 mMtxFrame;						// 1フレームでの変化を表す行列	
	Float3 mRotateStartPos;									// キー入力された際の開始位置	

	const int mMoveCnt = 12;							// 90度回転、押されるのに必要な更新回数
	int mCrrentRotCnt = 0;								// 今の回転回数	
	const float mRotAnglePerFrame = 90.0f / mMoveCnt;	// 1回当たりの回転角度	

	int mCrrentPushCnt = 0;								// 今の回転回数
	const float mPushPisitionPerFrame = DICE_SCALE / mMoveCnt;

	const int mUpCnt = 150;
	const float mUpPisitionPerFrame = DICE_SCALE / mUpCnt;

	// 上の面を特定
	DICETYPE OverPlane();
	// 回転移動セット
	void SetRollDirection(Direction _direction);

	void Up();
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
		//mTransform.SetScale(15.0f);
		AddComponent<Component::ModelComponent>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/dice/Dice.fbx"));
		AddComponent<Component::CollisionComponent>()->SetInitState(ObjectTag::DiceTop, Float3(0, 7, 0), Float3(8.5f, 8.5f*0.2f, 8.5f), DirectX::XMFLOAT4(0, 1, 0, 0.3f));
		AddComponent<Component::CollisionComponent>()->SetInitState(ObjectTag::Dice, Float3(0, -2, 0), Float3(8.5f, 8.5f*0.8f, 8.5f), DirectX::XMFLOAT4(0, 0, 1, 0.3f));
		ObjectInit();
	}
	~Dice()override;

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectDraw()override;
	void Uninit()override;

	// 初期面セット
	void SetInitType() {
		mTopDiceType = OverPlane();
	}

	// 指定方向に移動
	bool Push(Direction _direction);
	// 指定方向に回転
	bool Roll(Direction _direction);

	// 上面取得
	DICETYPE GetTopDiceType() {
		return mTopDiceType;
	}

	// モデルの状態を取得する(沈んでいるか)
	DICESTATUS GetDiceStatus() {
		return mSts;
	}

	Direction GetDirection() {
		return mDirection;
	}

	// 1マス分移動
	void MoveDiceScale(Direction _direction);

	void SetMapPos(INT3 _i3) { mMapPos = _i3; };
	INT3 GetMapPos() { return mMapPos; };

	void OnCollisionEnter(ComponentBase* _oher) override;
	void OnCollisionStay(ComponentBase* _oher) override;
	void OnCollisionExit(ComponentBase* _oher) override;

	float GetmPushPositionPerFrame() { return mPushPisitionPerFrame; }
	bool GetPush() { return mCrrentPushCnt == mMoveCnt; }
};

