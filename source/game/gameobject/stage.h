#pragma once
#include	"gameobject.h"
#include	<list>

class StageData;
class Stage : public GameObject
{
private:
	Dix::wp<StageData> mpCurrentStageData;
	INT2 mStagesize;

public:
	Stage();
	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectImguiDraw()override {};
	void ImguiCreateDraw()override;
	void Uninit() override {};

	void CameraUpdate();
	void Reset();
	INT2 GetStageSize() { return mStagesize; };
};

