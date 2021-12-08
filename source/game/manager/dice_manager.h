#pragma once
#include	"../gameobject/dice.h"
#include	"../gameobject/gameobject.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../manager/stagedata_manager.h"

class DiceManager :Uncopyable
{
private:
	std::vector<Dice*> mDiceList;
	StageData mCurrentStageData;
	int mDiceMap[STAGESIZEMAX][STAGESIZEMAX];		   // �u���b�N�X�V�p�}�b�v�z��(-1:�����@0�ȏ�:�u���b�N����,�ԍ��͐������ꂽ��)
	int mCheckMap[STAGESIZEMAX][STAGESIZEMAX];		   // �ڒn�u���b�N�`�F�b�N�p�}�b�v�z��
	bool mCheckboolMap[STAGESIZEMAX][STAGESIZEMAX];	   // �����Ă���u���b�N�̑��݊m�F�p�}�b�v�z��
	int mDiceAlignCnt = 0;							   // �`�F�b�N���ɃT�C�R������������ 

	// Dice����
	void DiceCreate();
	// �󂯎�����}�b�v�ʒu�Ɩʂ���ɃT�C�R�������������`�F�b�N���Ĕz�������������
	void CheckDiceAlign(INT3 _mapPos, DICEFRUIT _diceType);

public:
	static DiceManager* GetInstance() {
		static DiceManager Instance;
		return &Instance;
	}
	DiceManager() {};
	~DiceManager() {};

	// ������
	void Init();
	void Update();
	void Uninit();

	// Dice���ړ����鎖���\��
	bool CanDiceMove(Dice* _dice, Direction _dire);

	// �����Ă��邩
	void CheckAligned(Dice* _dice);
};

