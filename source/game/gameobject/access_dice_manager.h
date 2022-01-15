#pragma once
#include	"gameobject.h"

class AccessDiceManager : public GameObject
{
private:

public:
	AccessDiceManager();

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectDraw()override {};
	void ObjectImguiDraw()override;
	void Uninit()override {};
};

