#pragma once
#include	"../../system/util/uncopyable.h"
#include	"../../system/util/dixsmartptr.h"
#include	<unordered_map>
#include	"../data/stage_data.h"

class StageDataManager :public Uncopyable
{
private:
	std::unordered_map<
		std::string,					// キー(ステージ名)
		Dix::sp<StageData>				// データ(StageDataクラス)
	> mpStageHashmap;

	std::string mCurrentStageKey = "puzzle/init";		// 現在のステージを示すキー

public:
	static StageDataManager& GetInstance() {
		static StageDataManager Instance;
		return Instance;
	}

	// ステージ名セット
	bool SetCurrentStage(std::string _key);

	// 現在のステージデータのポインタ取得
	const Dix::wp<StageData> GetCurrentStage();

	// ステージデータ読み込み
	bool LoadStage(std::string _key);

	// ステージデータ保存
	void SaveStage(const StageData& _stagedata);

	// ステージデータのポインタを取得する
	const Dix::wp<StageData> GetStageData(std::string _key);

	// ステージデータを削除する
	void RemoveStageData(std::string _key);
};