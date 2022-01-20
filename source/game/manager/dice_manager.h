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

	int mDiceMap[STAGESIZEMAX][STAGESIZEMAX];			// ブロック更新用マップ配列(-1:無し　0以上:ブロックあり,番号は生成された順)
	int mCheckMap[STAGESIZEMAX][STAGESIZEMAX];			// 接地ブロックチェック用マップ配列
	bool mCheckboolMap[STAGESIZEMAX][STAGESIZEMAX];		// 揃っているブロックの存在確認用マップ配列
	int mDiceAlignCnt = 0;								// チェック中にサイコロが揃った数 
	int mFrameCnt = 0;
	Float3 mSpawnAngle[9];								// 生成するサイコロの角度パターン
	int mSpawnRate[6] = { 30,10,15,15,15,15 };			// 各サイコロが生成する割合(%)
	INT3 mPlayerPos;

	// Dice生成
	void DiceMapCreate();

	// 受け取ったマップ位置と面を基準にサイコロが揃ったかチェックして配列を書き換える
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

	// 初期化
	void Init();
	// 更新
	void Update();

	void ImguiDraw();

	void Uninit();

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
};