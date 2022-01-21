#pragma once
#include "scene_base.h"


class Stage;
class StageCreateScene : public SceneBase
{
private:

public:
	StageCreateScene();

	void SceneAfter() override;
	void SceneInit() override;
	void SceneUpdate() override;
};