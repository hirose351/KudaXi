#pragma once
#include	"state_base.h"

class GameModeBase :public StateBase
{
protected:
	//Component::PlayerController* mHolder;			// 持ち主

	/// Todo:アニメーション管理もここで宣言
public:
	//virtual ~GameModeBase() {};
	// 操作する変数登録
	//virtual void Start(Component::PlayerController* _c);
};
