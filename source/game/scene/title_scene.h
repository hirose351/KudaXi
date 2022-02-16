#pragma once
#include	"scene_base.h"
#include	"../gameobject/effect_fruit.h"
#include	"../gameobject/ui_button.h"

namespace Component
{
	class Animation2d;
}

class TitleScene : public SceneBase
{
private:
	Dix::wp<myUI::ButtonGroup> mpBg;		// ボタン
	Component::Animation2d* mpPlayerImage;	// プレイヤーの画像

public:
	TitleScene();

	void SceneAfter() override;
	void SceneInit() override;
	void SceneUpdate() override;
};

