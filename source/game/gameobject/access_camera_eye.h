#pragma once
#include	"gameobject.h"

class CameraEyeAccess : public GameObject
{
private:
	Float3 mBeforePos;

public:
	CameraEyeAccess();

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ImguiCreateDraw()override;
	void Uninit()override {};
};

