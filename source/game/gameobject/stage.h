#pragma once
#include	"gameobject.h"
#include	"../data/stage_data.h"
#include	"../../system/util/dixsmartptr.h"
#include	"plane.h"
#include	<list>

class Stage : public GameObject
{
private:
	StageData mCurrentStageData;
	Plane mPlane;

public:
	void ObjectInit() override;
	void ObjectUpdate()override {};
	void ObjectDraw()override;
	void Uninit() override {};
};

