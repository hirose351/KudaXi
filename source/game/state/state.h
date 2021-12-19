#pragma once
#include	"../../system/util/dixsmartptr.h"
#include	"../component/player_controller.h"

enum Pstate
{
	eStop,	// 実際は必要ない
	eMove,
	ePush,
	eRoll,
};

namespace Component
{
	class PlayerController;
}

class State
{
protected:
	Dix::wp<Transform> mTramsform;
	Dix::wp<Component::PlayerController> mHolder;
public:
	virtual ~State() {};
	// 操作する変数登録
	virtual void Start(Dix::wp<Component::PlayerController> _c);
	// 実行
	virtual void Exec() = 0;
	// State変更前処理
	virtual void BeforeChange() {}
	// State変更後処理
	virtual void AfterChange() {}
};