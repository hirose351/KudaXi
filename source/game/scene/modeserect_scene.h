#pragma once
#include	"scene_base.h"
#include	"../gameobject/effect_fruit.h"
#include	"../gameobject/ui_button.h"

class ModeSerectScene : public SceneBase
{
private:

public:
	ModeSerectScene();
	~ModeSerectScene();

	void SceneAfter() override;
	void SceneInit() override;
	void SceneUpdate() override;
	bool Dispose() override;
};
