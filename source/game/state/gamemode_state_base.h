#pragma once
#include	"state_base.h"
#include	"gamemode_controller.h"
#include	<vector>
class GameModeController;
class GameObject;
class Player;

class GameModeBase :public StateBase
{
protected:
	GameModeController* mHolder;			// 持ち主
	std::vector<Dix::wp<GameObject>> mModeObjList;	// モードごとに使用するオブジェクト(コンストラクタで生成,アクティブ状態管理)
	Dix::wp<Player> mPlayer;

public:
	virtual ~GameModeBase() {};
	// 操作する変数登録
	virtual void Start(GameModeController* _c) { mHolder = _c; };
};
