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
		int							  mStateNum;			// ���݂̃X�e�[�g�ԍ�
		Direction					  mDiceMoveDirection;	// �T�C�R������]���������
		Dix::sp<Direction>			  mDirection;			// �v���C���[�̕���(�L�[�Q��)
		Dix::wp<GameObject>			  mDiceModel;			// ���삵�Ă���T�C�R�����f��
		Dix::wp<GameObject>			  mDiceBg;				// ���삵�Ă���T�C�R���w�i
		Dix::wp<StageData> 			  mStageData;			// �X�e�[�W���
		Foot						  mFoot;				// �����̃I�u�W�F�N�g
		Float3						  mInfoDicePos;			// ��ʂɕ\������ʒu
		bool mIsDiceUiDraw = false;							// ����UI�̃T�C�R����`�悷�邩

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

		/// �A�N�Z�T //////////////////////////////////////////////////////////////////////////////////

		int GetCurrentState() { return mStateNum; }

		Dix::wp<Direction> GetDirection() { return mDirection; }
		Foot* GetFoot() { return &mFoot; };
		INT3 GetStageSize() { return (mStageData->mMapSizeWidth, 0, mStageData->mMapSizeHeight); }
	};
}
