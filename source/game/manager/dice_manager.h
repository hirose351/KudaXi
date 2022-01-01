#pragma once
#include	"../gameobject/dice.h"
#include	"../gameobject/gameobject.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../manager/stagedata_manager.h"
#include	<random>

class DiceManager :Uncopyable
{
private:
	std::random_device rnd;     // �񌈒�I�ȗ���������𐶐�
	// ��������T�C�R���̊p�x�p�^�[��
	Float3 randAngle[6] = { { 90.0f,0,0},{-90.0f,0,0},{180.0f,0,0} ,{0,90.0f,0} ,{0,180.0f,90.0f },{90.0f,90.0f,0} };
	std::vector<Dice*> mDiceList;
	StageData mCurrentStageData;
	int mDiceMap[STAGESIZEMAX][STAGESIZEMAX];		   // �u���b�N�X�V�p�}�b�v�z��(-1:�����@0�ȏ�:�u���b�N����,�ԍ��͐������ꂽ��)
	int mCheckMap[STAGESIZEMAX][STAGESIZEMAX];		   // �ڒn�u���b�N�`�F�b�N�p�}�b�v�z��
	bool mCheckboolMap[STAGESIZEMAX][STAGESIZEMAX];	   // �����Ă���u���b�N�̑��݊m�F�p�}�b�v�z��
	int mDiceAlignCnt = 0;							   // �`�F�b�N���ɃT�C�R������������ 

	// Dice����
	void DiceMapCreate();
	// �󂯎�����}�b�v�ʒu�Ɩʂ���ɃT�C�R�������������`�F�b�N���Ĕz�������������
	void CheckDiceAlign(INT3 _mapPos, DiceFruit _diceType);

	void DiceRondomAdd();

	void DiceCreate();

	Dice* GetListInDice(int x, int z);

public:
	static DiceManager* GetInstance() {
		static DiceManager Instance;
		return &Instance;
	}
	DiceManager() {};
	~DiceManager() {};

	// ������
	void Init();
	// �X�V
	void Update();
	// 
	void Uninit();

	// Dice���ړ����鎖���\��
	bool CanDiceMove(Dice* _dice, Direction _dire);

	// Dice���ړ������悪�s���~�܂肩
	bool CanDiceMoveCheak(Dice* _dice, Direction _dire);

	// �����Ă��邩
	void CheckAligned(Dice* _dice);

	// �Ώۂ̃T�C�R���̃}�b�v��̃f�[�^������
	void SetRemoveDice(Dice* _dice);

	// �n���ꂽ�}�b�v��ɂ���Dice�̃|�C���^��Ԃ�
	Dice* GetDice(INT3 _mapPos);
};