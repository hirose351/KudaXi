#pragma once
#include "IScene.h"

class TitleScene : public IScene
{
private:

public:
	bool Init() override;
	void Update() override;
	void Render() override;
	bool Dispose() override;
};

