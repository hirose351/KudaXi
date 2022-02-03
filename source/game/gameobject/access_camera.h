#pragma once
#include	"gameobject.h"

class CameraAccess : public GameObject
{
private:

public:
	CameraAccess();

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ImguiCreateDraw()override;
	void Uninit()override {};
};

