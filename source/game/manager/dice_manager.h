#pragma once
#include	"../gameobject/dice.h"
#include	"../gameobject/gameobject.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../manager/stagedata_manager.h"
#include	<random>

class DiceManager :Uncopyable
{
private:
	std::vector<Dix::wp<Dice>> mpDiceList;
	Dix::wp<StageData> mCurrentStageData;

	int mDiceMap[STAGESIZEMAX][STAGESIZEMAX];			// �u���b�N�X�V�p�}�b�v�z��(-1:�����@0�ȏ�:�u���b�N����,�ԍ��͐������ꂽ��)
	int mCheckMap[STAGESIZEMAX][STAGESIZEMAX];			// �ڒn�u���b�N�`�F�b�N�p�}�b�v�z��
	bool mCheckboolMap[STAGESIZEMAX][STAGESIZEMAX];		// �����Ă���u���b�N�̑��݊m�F�p�}�b�v�z��
	int mDiceAlignCnt = 0;								// �`�F�b�N���ɃT�C�R������������ 
	int mFrameCnt = 0;
	Float3 mSpawnAngle[9];								// ��������T�C�R���̊p�x�p�^�[��
	int mSpawnRate[6] = { 30,10,15,15,15,15 };			// �e�T�C�R�����������銄��(%)
	INT3 mPlayerPos;

	// Dice����
	void DiceMapCreate();

	// �󂯎�����}�b�v�ʒu�Ɩʂ���ɃT�C�R�������������`�F�b�N���Ĕz�������������
	void CheckDiceAlign(INT3 _mapPos, DiceFruit _diceType);

	Dix::wp<Dice> GetListInDice(int x, int z);

	int GetDiceRandomNum(int _rndNum);

public:
	static DiceManager* GetInstance() {
		static DiceManager Instance;
		return &Instance;
	}
	DiceManager();
	~DiceManager() {};

	// ������
	void Init();
	// �X�V
	void Update();

	void ImguiDraw();

	void Uninit();

	// Dice���ړ����鎖���\��
	bool CanDiceMove(Dice* _dice, Direction _dire);

	// Dice���ړ������悪�s���~�܂肩
	bool CanDiceMoveCheak(Dice* _dice, Direction _dire);

	// �����Ă��邩
	void CheckAligned(Dice* _dice);

	// �Ώۂ̃T�C�R���̃}�b�v��̃f�[�^������
	void SetRemoveDice(int _diceId);

	// �n���ꂽ�}�b�v��ɂ���Dice�̃|�C���^��Ԃ�
	Dix::wp<Dice> GetDice(INT3 _mapPos);

	void SetPlayerPos(INT3 _pos) { mPlayerPos = _pos; };
};