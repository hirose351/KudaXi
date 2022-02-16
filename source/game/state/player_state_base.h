#pragma once
#include	"../../system/util/dixsmartptr.h"
#include	"../component/player_controller.h"
#include	"../gameobject/transform.h"
#include	"../manager/dice_manager.h"
#include	"state_base.h"

// 状態
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
	Component::PlayerController* mHolder;			// 持ち主
	Dix::wp<Transform>			 mTransform;		// 位置回転大きさ
	Dix::wp<Direction>			 mDirection;		// 移動方向
	Dix::wp<Dice>				 mpOperationDice;	// 操作するDice
	Foot*						 mFoot;				// 足元
	INT3						 mMapPos;			// マップ上の位置

	/// Todo:アニメーション管理もここで管理したい

public:
	virtual ~PlayerStateBase() {};

	// 操作する変数登録
	virtual void Start(Component::PlayerController* _c);


	/// アクセサ ////////////////////////////////////////////////////////////////

	void SetOperationDice(Dix::wp<Dice> _dice) { mpOperationDice = _dice; }
	Dix::wp<Dice> GetOperationDice() { return mpOperationDice; }

	INT3 GetMapPos() { return mMapPos; }
	void SetMapPos(INT3 _pos) { mMapPos = _pos; }
};