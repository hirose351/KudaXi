#pragma once
#include "scene_base.h"


class Stage;
class StageCreateScene : public SceneBase
{
private:
	std::vector<Dix::wp<GameObject>> mViewObjList;	// クリエイト表示するオブジェクトリスト

public:
	StageCreateScene();

	void SceneAfter() override;
	void SceneInit() override;
	void SceneUpdate() override;
	void ImguiDebug() override;
};