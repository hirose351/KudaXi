#pragma once
#include	"gameobject.h"

class DiceManagerAccess : public GameObject
{
private:

public:
	DiceManagerAccess();

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectImguiDraw()override;
	void Uninit()override {};
};

