#pragma once
#include	"../../system/util/dixsmartptr.h"
#include	"gamemode_state_base.h"
#include	"../component/component_base.h"
#include	"../gameobject/player.h"
#include	"../gameobject/stage.h"
#include	<map>

#define TOTALSTAGE (10)

class GameModeBase;

namespace Component {
	class GameModeController :public ComponentBase
	{
	private:
		std::map<int, Dix::sp<GameModeBase>> mStates;
		int mModeNum;
		int mSelectStage;
		Dix::wp<Player> mpPlayer;
		Dix::wp<Stage> mpStage;

	public:
		GameModeController();
		~GameModeController();

		void Awake()override {};
		void Init()override;
		void Update()override;
		void ImguiDraw()override;
		void Uninit()override {};

		void Init(int _modeNum);

		void ChangeMode(int _modeNum);

		int GetSelectStage() { return mSelectStage; }
		void GetSelectStage(int _num) { mSelectStage = mSelectStage; }

		Dix::wp<Player> GetPlayer() { return mpPlayer; };
		void SetPlayer(Dix::wp<Player> _player) { mpPlayer = _player; };
		Dix::wp<Stage> GetStage() { return mpStage; };
		void SetStage(Dix::wp<Stage> _stage) { mpStage = _stage; };
	};
}