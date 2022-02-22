#pragma once
#include "scene_base.h"
#include "../data/stage_data.h"

class Stage;
class StageCreateScene : public SceneBase
{
private:
	std::vector<Dix::wp<GameObject>> mpViewObjList;	// クリエイト表示するオブジェクトリスト

	char mStageNameText[128] = {};	// プレイヤーが入力する用ステージ名
	Dix::wp<StageData> mpStageData;
	Dix::wp<Stage> mpStage;
	int mSelectObjNum;				// 選択されているオブジェクトの数字
	bool mIsPlay = false;			// プレイしているか
	bool mIsStagePlay = false;		// ステージデータをプレイしているか
	std::string mBeforeStageName;	// プレイを選択した時に入力されていたステージ名

	void StageDataSave();
	void StageDataLoad();
	void StageDataPlay();
	void StagePlay();
	void RuleImGuiDraw();

public:
	StageCreateScene();

	void SceneAfter() override;
	void SceneInit() override;
	void SceneUpdate() override;
	void ImguiDebug() override;
};