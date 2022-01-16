#pragma once
#include	"gameobject.h"
#include	"../manager/stagedata_manager.h"
#include	"../component/plane.h"
#include	<list>

class Stage : public GameObject
{
private:
	StageData mCurrentStageData;
	Component::Plane* mpPlane;

public:
	Stage();
	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectImguiDraw()override {};
	void Uninit() override {};
};

