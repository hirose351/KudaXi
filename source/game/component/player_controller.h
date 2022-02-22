#pragma once
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
		std::map<int, Dix::sp<PlayerStateBase>> mpStatesList;
		int							  mStateNum;				// 現在のステート番号
		Direction					  mDiceMoveDirection;		// サイコロを回転させる方向
		Dix::sp<Direction>			  mpDirection;				// プレイヤーの方向(キー参照)
		Dix::wp<GameObject>			  mpDiceModel;				// 操作しているサイコロモデル
		Dix::wp<GameObject>			  mpDiceBg;					// 操作しているサイコロ背景
		Dix::wp<StageData> 			  mpStageData;				// ステージ情報
		Foot						  mFoot;					// 足元のオブジェクト
		Float3						  mInfoDicePos;				// 画面に表示する位置
		bool mIsDiceUiDraw = false;								// 左上UIのサイコロを描画するか

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

		Dix::wp<Direction> GetDirection() { return mpDirection; }
		Foot* GetFoot() { return &mFoot; };
		INT2 GetStageSize() { return INT2(mpStageData->mMapSizeWidth, mpStageData->mMapSizeHeight); }
	};
}
