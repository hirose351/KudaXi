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
		int							  mStateNum;			// 現在のステート番号
		Direction					  mDiceMoveDirection;	// サイコロを回転させる方向
		Dix::sp<Direction>			  mDirection;			// プレイヤーの方向(キー参照)
		Dix::wp<GameObject>			  mDiceModel;			// 操作しているサイコロモデル
		Dix::wp<GameObject>			  mDiceBg;				// 操作しているサイコロ背景
		Dix::wp<StageData> 			  mStageData;			// ステージ情報
		Foot						  mFoot;				// 足元のオブジェクト
		Float3						  mInfoDicePos;			// 画面に表示する位置
		bool mIsDiceUiDraw = false;							// 左上UIのサイコロを描画するか

	public:
		PlayerController();
		~PlayerController();

		void Awake()override {};
		void Init()override;
		void Update()override;
		void ImguiDraw()override;
		void Uninit()override {};

		void SetDiceUi();
		void RemoveDiceUi();

		void StateInit(int _stateNum);
		void ChangeState(int _stateNum);

		/// アクセサ //////////////////////////////////////////////////////////////////////////////////

		int GetCurrentState() { return mStateNum; }

		Dix::wp<Direction> GetDirection() { return mDirection; }
		Foot* GetFoot() { return &mFoot; };
		INT3 GetStageSize() { return (mStageData->mMapSizeWidth, 0, mStageData->mMapSizeHeight); }
	};
}
