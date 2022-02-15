#pragma once
#include	"scene_base.h"

namespace myUI {
	class ButtonGroup;
}

class ModeSelectScene : public SceneBase
{
private:
	Dix::wp<myUI::ButtonGroup> mpBg;
	Dix::wp<GameObject> mpRuleImage;
	bool mIsRuleDraw = false;

public:
	ModeSelectScene();

	void SceneAfter() override;
	void SceneInit() override;
	void SceneUpdate() override;
};
