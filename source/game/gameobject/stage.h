#pragma once
#include	"gameobject.h"
#include	"../manager/stagedata_manager.h"
#include	"../component/plane.h"
#include	<list>

class Stage : public GameObject
{
private:
	Dix::wp<StageData> mCurrentStageData;
	void CameraUpdate();
public:
	Stage();
	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectImguiDraw()override {};
	void ImguiCreateDraw()override;
	void Uninit() override {};
};

