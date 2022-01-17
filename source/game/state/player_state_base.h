#pragma once
#include	"../../system/util/dixsmartptr.h"
#include	"../component/player_controller.h"
#include	"../gameobject/transform.h"
#include	"../manager/dice_manager.h"
#include	"state_base.h"

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

class PlayerStateBase :public StateBase
{
protected:
	//Dix::wp<Component::PlayerController> mHolder;			// 持ち主
	Component::PlayerController* mHolder;			// 持ち主
	Dix::wp<Transform>			 mTransform;		// 位置回転大きさ
	Dix::wp<Direction>			 mDirection;		// 移動方向
	Dice*						 mpOperationDice;	// 操作するDice
	Foot*						 mFoot;				// 足元
	INT3						 mMapPos;			// マップ上の位置

	/// Todo:アニメーション管理もここで宣言
public:
	virtual ~PlayerStateBase() {};
	// 操作する変数登録
	virtual void Start(Component::PlayerController* _c);

	void SetOperationDice(Dice* _dice) { mpOperationDice = _dice; }
	Dice* GetOperationDice() { return mpOperationDice; }

	INT3 GetMapPos() { return mMapPos; }
	void SetMapPos(INT3 _pos) { mMapPos = _pos; }
};