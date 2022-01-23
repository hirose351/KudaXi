#pragma once
#include	"gameobject.h"

class DiceCreateManagerAccess : public GameObject
{
private:

public:
	DiceCreateManagerAccess();

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ImguiCreateDraw()override;
	void Uninit()override {};
};

