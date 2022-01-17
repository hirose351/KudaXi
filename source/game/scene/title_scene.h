#pragma once
#include	"scene_base.h"
#include	"../gameobject/effect_fruit.h"
#include	"../gameobject/ui_button.h"

class TitleScene : public SceneBase
{
private:
	Effect::Fruit* mpFruit;
	GameObject* mpObj;
	myUI::ButtonGroup* mpBg;
	bool mIsButtonPush = false;
public:
	TitleScene();

	void SceneAfter() override;
	void SceneInit() override;
	void SceneUpdate() override;
};

