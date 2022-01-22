#pragma once
#include "scene_base.h"


class Stage;
class StageCreateScene : public SceneBase
{
private:
	std::vector<Dix::wp<GameObject>> mViewObjList;	// �N���G�C�g�\������I�u�W�F�N�g���X�g
	char mStageNameText[128] = {};
	int mSelectObjNum;

	void StageDataSave();
	void StageDataLoad();
	void StageDataPlay();

public:
	StageCreateScene();

	void SceneAfter() override;
	void SceneInit() override;
	void SceneUpdate() override;
	void ImguiDebug() override;
};