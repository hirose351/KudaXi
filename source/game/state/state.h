#pragma once
#include	"../../system/util/dixsmartptr.h"
#include	"../component/player_controller.h"
#include	"../gameobject/transform.h"
#include	"../manager/dice_manager.h"

enum
{
	eMove,
	ePush,
	eRoll,
};


namespace Component
{
	class PlayerController;
}

class DIce;

class State
{
protected:
	//Dix::wp<Component::PlayerController> mHolder;			// 持ち主
	Component::PlayerController* mHolder;			// 持ち主
	Dix::wp<Transform>				 mTransform;			// 位置回転大きさ
	Dix::wp<Direction>				 mDirection;			// 移動方向
	Dice*				 mpOperationDice;					// 移動方向
	Foot*				 mFoot;			// 移動方向
	INT3 mMapPos;										// マップ上の位置

	/// Todo:アニメーション管理もここで宣言
public:
	virtual ~State() {};
	// 操作する変数登録
	virtual void Start(Component::PlayerController* _c);
	// 初期処理
	virtual void Init() {};
	// 実行
	virtual void Exec() = 0;
	// State変更前処理
	virtual void BeforeChange() {}
	// State変更後処理
	virtual void AfterChange() {}
	// 
	void SetOperationDice(Dice* _dice) { mpOperationDice = _dice; }
	Dice* GetOperationDice() { return mpOperationDice; }
};