#pragma once
#include		"gameobject.h"
#include		"../manager/stagedata_manager.h"

using Microsoft::WRL::ComPtr;

class Player :public GameObject
{
private:

public:
	Player();
	~Player();

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectImguiDraw()override;
	void Uninit() override;
};