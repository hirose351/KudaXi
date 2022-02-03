#pragma once
#include	"gameobject.h"

class CameraAccessLookat : public GameObject
{
private:
	Float3 mBeforePos;

public:
	CameraAccessLookat();

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ImguiCreateDraw()override;
	void Uninit()override {};
};

