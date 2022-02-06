#pragma once
#include	"gameobject.h"
#include	"../manager/stagedata_manager.h"
#include	"../component/plane.h"
#include	<list>

class Stage : public GameObject
{
private:
	Dix::wp<StageData> mCurrentStageData;
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

