#pragma once
#include	"../gameobject/dice.h"
#include	"../manager/stagedata_manager.h"
#include	<random>

#define		NODICE	(-1)

class DiceManager :Uncopyable
{
private:
	std::vector<Dix::wp<Dice>> mpDiceList;
	Dix::wp<StageData> mpCurrentStageData;

	int mDiceMap[STAGESIZEMAX][STAGESIZEMAX];			// �u���b�N�X�V�p�}�b�v�z��(-1:�����@0�ȏ�:�u���b�N����,�ԍ��͐������ꂽ��)
	int mCheckMap[STAGESIZEMAX][STAGESIZEMAX];			// �ڒn�u���b�N�`�F�b�N�p�}�b�v�z��
	bool mCheckboolMap[STAGESIZEMAX][STAGESIZEMAX];		// �����Ă���u���b�N�̑��݊m�F�p�}�b�v�z��
	int mDiceAlignCnt = 0;								// �`�F�b�N���ɃT�C�R������������ 
	int mFrameCnt = 0;
	Float3 mSpawnAngle[9];								// ��������T�C�R���̊p�x�p�^�[��
	int mSpawnRate[6] = { 35,5,15,15,15,15 };			// �e�T�C�R�����������銄��(%)
	INT3 mPlayerPos;

	/// ��Create�p ////////////////////

	int mSelectNum;
	bool mIsSelect = false;

	bool CreateAddDice();

	Dix::wp<Dice> GetCreateListInDice(int x, int z);

	/// ��Create�p ////////////////////


	/// ��Puzzle�p ////////////////////

	int mStepCount;				// Dice����������
	bool mIsStepCount = false;	// �X�e�b�v�𐔂��邩

	/// ��Puzzle�p ////////////////////

	// �󂯎�����}�b�v�ʒu�Ɩʂ���ɃT�C�R�������������`�F�b�N���Ĕz�������������
	void CheckDiceAlign(INT3 _mapPos, DiceFruit _diceType);

	Dix::wp<Dice> GetListInDice(int x, int z);

	int GetDiceRandomNum(int _rndNum);

	int mEndlessCnt = 0;		// ����������
	int mScore = 0;				// ���X�R�A

	// �����o����}�X��Ԃ�
	INT2 GetSpawnPos(int _rndNum);
	// �����_�������ʒu��Ԃ�
	INT2 GetRandomSpawnPos(int _rndNum);

public:
	static DiceManager* GetInstance() {
		static DiceManager Instance;
		return &Instance;
	}
	DiceManager();
	~DiceManager() {};

	void DiceMapCreate(bool _isUp);

	void ImguiDraw();

	void Uninit();

	void SetPuzzle();

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

	/// ��Create�p ////////////////////

	void CreateInit();
	void CreateUpdate();
	void CreateImguiDraw();

	void DataCreate();

	INT2 GetMoveMapPos(Direction _direction, INT2 _mapPos);

	void SetIsSelect(bool _flg) { mIsSelect = _flg; }

	// �Ώۂ̃T�C�R���̃}�b�v��̃f�[�^������
	void SetCreateRemoveDice(int _diceId);

	// �n���ꂽ�}�b�v��ɂ���Dice�̃|�C���^��Ԃ�
	Dix::wp<Dice> GetCreateDice(INT2 _mapPos);

	// �}�b�v��Dice�̒l������
	void SetCreateDiceMap();

	/// ��Create�p ////////////////////

	/// ��Puzzle�p ////////////////////

	void PuzzleInit();
	int GetStepCount() { return mStepCount; };
	void SetIsStepCount(bool _flg);
	bool GetIsAllAligned();

	/// ��Puzzle�p ////////////////////

	void EndlessInit();
	void EndleesUpdate();
	bool GetEndlessIsOver();
};