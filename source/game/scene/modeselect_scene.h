#pragma once
#include	"scene_base.h"

namespace myUI {
	class ButtonGroup;
}

class ModeSelectScene : public SceneBase
{
private:
	Dix::wp<myUI::ButtonGroup> mpBg;	// ボタン
	Dix::wp<GameObject> mpRuleImage;	// ルール画像
	bool mIsRuleDraw = false;			// ルールを表示するか

public:
	ModeSelectScene();

	void SceneAfter() override;
	void SceneInit() override;
	void SceneUpdate() override;
};
