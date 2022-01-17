#pragma once
#include "scene_base.h"

class StageCreateScene : public SceneBase
{
private:

public:
	StageCreateScene();
	~StageCreateScene();

	void SceneAfter() override;
	void SceneInit() override;
	void SceneUpdate() override;
	bool Dispose() override;
};