#pragma once
#include	"gameobject.h"

class DiceEndlessManagerAccess : public GameObject
{
private:

public:
	DiceEndlessManagerAccess();

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectImguiDraw()override;
	void Uninit()override {};
};

