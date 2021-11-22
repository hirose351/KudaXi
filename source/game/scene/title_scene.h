#pragma once
#include "scene_base.h"

class TitleScene : public SceneBase
{
private:

public:
	bool Init() override;
	void Update() override;
	void Render() override;
	bool Dispose() override;
};

