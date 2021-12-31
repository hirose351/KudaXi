#pragma once
#include "scene_base.h"

class TitleScene : public SceneBase
{
private:



public:
	TitleScene();
	~TitleScene();

	void SceneInit() override;
	void SceneUpdate() override;
	void SceneRender() override;
	/*void ImguiDebug() override;*/
	bool Dispose() override;
};

