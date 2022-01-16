#pragma once
#include		"../../system/util/dixsmartptr.h"
#include		"../component/component_base.h"
#include		"../manager/stagedata_manager.h"
#include		"../gameobject/transform.h"
#include		"../gameobject/gameobject_utility.h"
#include		"../state/player_state_base.h"
#include		<map>

class PlayerStateBase;
class Dice;

namespace Component {
	class PlayerController :public ComponentBase
	{
	private:
		std::map<int, Dix::sp<PlayerStateBase>> mStates;
		int							  mStateNum;
		Direction					  mDiceMoveDirection; // サイコロを回転させる方向
		Dix::sp<Direction>			  mDirection;		  // プレイヤーの方向(キー参照)
		StageData					  stageData;		  // ステージ情報
		Foot						  mFoot;

	public:
		PlayerController();
		~PlayerController();

		void Awake()override {};
		void Init()override;
		void Update()override;
		void ImguiDraw()override;
		void Uninit()override {};

		void ChangeState(int _stateNum);
		int GetCurrentState() { return mStateNum; }

		Dix::wp<Direction> GetDirection() { return mDirection; }
		Foot* GetFoot() { return &mFoot; };
		INT3 GetStageSize() { return (stageData.mMapSizeWidth, 0, stageData.mMapSizeHeight); }
	};
}
