#pragma once
#include	"../../system/util/uncopyable.h"
#include	<memory>
#include	<wrl/client.h>
#include	<unordered_map>
#include	"../data/stage_data.h"

class StageDataManager :public Uncopyable
{
private:
	StageDataManager() {
	}
	std::unordered_map<
		std::string,					// キー(ステージ名)
		std::unique_ptr<StageData>		// データ(StageDataクラス)
	> mStageHashmap;

	std::string mCurrentStageKey = "puzzle/init";		// 現在のステージを示すキー

public:
	static StageDataManager& GetInstance() {
		static StageDataManager Instance;
		return Instance;
	}

	virtual ~StageDataManager() {
		//	Finalize();
	}

	// 後始末
	void Finalize();

	// ステージ名セット
	bool SetCurrentStage(std::string _key);

	// 現在のステージデータのポインタ取得
	const StageData* GetCurrentStage();

	// ステージデータ読み込み
	bool LoadStage(std::string _key);

	// ステージデータ保存
	void SaveStage(const StageData& _stagedata);

	// ステージデータのポインタを取得する
	const StageData* GetStageData(std::string _key);

	// ステージデータを削除する
	void RemoveStageData(std::string _key);
};