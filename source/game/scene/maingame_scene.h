#pragma once
#include	"scene_base.h"

class MaingameScene : public SceneBase
{
private:

public:
	MaingameScene();

	void SceneAfter() override;
	void SceneInit() override;
	void SceneUpdate() override;
};