#pragma once
#include	"../gameobject/dice.h"
#include	"../gameobject/gameobject.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../manager/stagedata_manager.h"
#include	<random>

class DiceManager :Uncopyable
{
private:
	std::vector<Dice*> mDiceList;
	StageData mCurrentStageData;
	int mDiceMap[STAGESIZEMAX][STAGESIZEMAX];		   // ブロック更新用マップ配列(-1:無し　0以上:ブロックあり,番号は生成された順)
	int mCheckMap[STAGESIZEMAX][STAGESIZEMAX];		   // 接地ブロックチェック用マップ配列
	bool mCheckboolMap[STAGESIZEMAX][STAGESIZEMAX];	   // 揃っているブロックの存在確認用マップ配列
	int mDiceAlignCnt = 0;							   // チェック中にサイコロが揃った数 

	// Dice生成
	void DiceCreate();
	// 受け取ったマップ位置と面を基準にサイコロが揃ったかチェックして配列を書き換える
	void CheckDiceAlign(INT3 _mapPos, DICEFRUIT _diceType);

	std::random_device rnd;     // 非決定的な乱数生成器を生成
public:
	static DiceManager* GetInstance() {
		static DiceManager Instance;
		return &Instance;
	}
	DiceManager() {};
	~DiceManager() {};

	// 初期化
	void Init();
	void Update();
	void Uninit();

	// Diceを移動する事が可能か
	bool CanDiceMove(Dice* _dice, Direction _dire);

	// Diceが移動した先が行き止まりか
	bool CanDiceMoveCheak(Dice* _dice, Direction _dire);

	// 揃っているか
	void CheckAligned(Dice* _dice);

	// 対象のサイコロのマップ上のデータを消す
	void SetRemoveDice(Dice* _dice);

	// 操作可能なサイコロのアドレスを返す
	//Dice* GetNearestDice(Float3 _pos);
};