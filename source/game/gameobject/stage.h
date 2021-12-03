#pragma once
#include	"gameobject.h"
#include	"../manager/stagedata_manager.h"
#include	"../../system/util/dixsmartptr.h"
#include	"plane.h"
#include	<list>

class Stage : public GameObject
{
private:
	StageData mCurrentStageData;
	Plane mPlane;

public:
	Stage() :GameObject(("Stage"), ObjectType::Obstracle) {};
	void ObjectInit() override;
	void ObjectUpdate()override {};
	void ObjectDraw()override;
	void Uninit() override {};
};

