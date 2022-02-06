#pragma once
#include "scene_base.h"
#include "../data/stage_data.h"

class Stage;
class StageCreateScene : public SceneBase
{
private:
	std::vector<Dix::wp<GameObject>> mViewObjList;	// クリエイト表示するオブジェクトリスト

	char mStageNameText[128] = {};
	Dix::wp<StageData> mStageData;
	Dix::wp<Stage> mStage;
	int mSelectObjNum;

	void RuleImGuiDraw();

	void StageDataSave();
	void StageDataLoad();
	void StageDataPlay();
	void Play();

public:
	StageCreateScene();

	void SceneAfter() override;
	void SceneInit() override;
	void SceneUpdate() override;
	void ImguiDebug() override;
};