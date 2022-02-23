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

	int mDiceMap[STAGESIZEMAX][STAGESIZEMAX];			// ブロック更新用マップ配列(-1:無し　0以上:ブロックあり,番号は生成された順)
	int mCheckMap[STAGESIZEMAX][STAGESIZEMAX];			// 接地ブロックチェック用マップ配列
	bool mCheckboolMap[STAGESIZEMAX][STAGESIZEMAX];		// 揃っているブロックの存在確認用マップ配列
	int mDiceAlignCnt = 0;								// チェック中にサイコロが揃った数 
	int mFrameCnt = 0;
	Float3 mSpawnAngle[9];								// 生成するサイコロの角度パターン
	int mSpawnRate[6] = { 35,5,15,15,15,15 };			// 各サイコロが生成する割合(%)
	INT3 mPlayerPos;

	/// ↓Create用 ////////////////////

	int mSelectNum;
	bool mIsSelect = false;

	bool CreateAddDice();

	Dix::wp<Dice> GetCreateListInDice(int x, int z);

	/// ↑Create用 ////////////////////


	/// ↓Puzzle用 ////////////////////

	int mStepCount;				// Diceが動いた回数
	bool mIsStepCount = false;	// ステップを数えるか

	/// ↑Puzzle用 ////////////////////

	// 受け取ったマップ位置と面を基準にサイコロが揃ったかチェックして配列を書き換える
	void CheckDiceAlign(INT3 _mapPos, DiceFruit _diceType);

	Dix::wp<Dice> GetListInDice(int x, int z);

	int GetDiceRandomNum(int _rndNum);

	int mEndlessCnt = 0;		// 初期生成数
	int mScore = 0;				// 総スコア

	// 生成出来るマスを返す
	INT2 GetSpawnPos(int _rndNum);
	// ランダム生成位置を返す
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

	// Diceを移動する事が可能か
	bool CanDiceMove(Dice* _dice, Direction _dire);

	// Diceが移動した先が行き止まりか
	bool CanDiceMoveCheak(Dice* _dice, Direction _dire);

	// 揃っているか
	void CheckAligned(Dice* _dice);

	// 対象のサイコロのマップ上のデータを消す
	void SetRemoveDice(int _diceId);

	// 渡されたマップ上にあるDiceのポインタを返す
	Dix::wp<Dice> GetDice(INT3 _mapPos);

	void SetPlayerPos(INT3 _pos) { mPlayerPos = _pos; };

	/// ↓Create用 ////////////////////

	void CreateInit();
	void CreateUpdate();
	void CreateImguiDraw();

	void DataCreate();

	INT2 GetMoveMapPos(Direction _direction, INT2 _mapPos);

	void SetIsSelect(bool _flg) { mIsSelect = _flg; }

	// 対象のサイコロのマップ上のデータを消す
	void SetCreateRemoveDice(int _diceId);

	// 渡されたマップ上にあるDiceのポインタを返す
	Dix::wp<Dice> GetCreateDice(INT2 _mapPos);

	// マップにDiceの値を入れる
	void SetCreateDiceMap();

	/// ↑Create用 ////////////////////

	/// ↓Puzzle用 ////////////////////

	void PuzzleInit();
	int GetStepCount() { return mStepCount; };
	void SetIsStepCount(bool _flg);
	bool GetIsAllAligned();

	/// ↑Puzzle用 ////////////////////

	void EndlessInit();
	void EndleesUpdate();
	bool GetEndlessIsOver();
};