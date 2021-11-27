#pragma once
#include	"gameobject.h"
#include	"collision.h"
#include	"../../system/model/ModelMgr.h"

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
	DOWN,		// 沈む
	HALFDOWN,	// 半分沈む
};

class Dice : public GameObject, CollisionInterface
{
private:
	CModel* mpModel;
	DICETYPE mTopDiceType;								// 上面
	DICESTATUS mSts;	// 生存状態
	DIRECTION mDirection = DIRECTION::NEUTRAL;			// サイコロの移動方向	

	DirectX::XMFLOAT4X4 mMtxFrame;						// 1フレームでの変化を表す行列	
	Float3 mStartPos;									// キー入力された際の開始位置	

	int mCrrentRotCnt = 0;								// 今の回転回数
	const int mRotCnt = 12;								// 90度回転するのに必要な更新回数	
	const float mRotAnglePerFrame = 90.0f / mRotCnt;	// 1回当たりの回転角度	

	// 上の面を特定
	DICETYPE OverPlane();
	// 回転移動セット
	void SetRollDirection(DIRECTION _direction);

	// モデルポインタ取得
	void SetModel(CModel* p) {
		mpModel = p;
	}
public:
	Dice() :GameObject(("Dice"), ObjectType::Dice) {
		bool sts = ModelMgr::GetInstance().LoadModel(
			"assets/model/dice/Dice.fbx",
			"shader/vs.hlsl", "shader/pstexcol.hlsl",
			"assets/model/dice/");
		if (!sts)
		{
			MessageBox(nullptr, "Diceモデル 読み込みエラー", "error", MB_OK);
		}
		SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/dice/Dice.fbx"));
		Init();
	}
	~Dice()override;

	void Init() override;
	void Update()override;
	void Draw()override;
	void Finalize()override;

	// 初期面セット
	void SetInitType() {
		mTopDiceType = OverPlane();
	}

	// 指定方向に移動
	bool Push(DIRECTION _direction);
	// 指定方向に回転
	bool Roll(DIRECTION _direction);

	// 上面取得
	DICETYPE GetTopDiceType() {
		return mTopDiceType;
	}

	// モデルの状態を取得する(沈んでいるか)
	DICESTATUS GetDiceStatus() {
		return mSts;
	}

	DIRECTION GetDirection() {
		return mDirection;
	}

	// 1マス分移動
	void MoveDiceScale(DIRECTION _direction);

	void OnCollisionEnter(GameObject* _oher) override;
	void OnCollisionStay(GameObject* _oher) override;
	void OnCollisionExit(GameObject* _oher) override;
};

