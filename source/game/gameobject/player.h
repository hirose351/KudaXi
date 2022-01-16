#pragma once
#include		"gameobject.h"
#include		"../manager/stagedata_manager.h"

using Microsoft::WRL::ComPtr;

class Player :public GameObject
{
private:
	StageData	mStageData;

public:
	Player();
	~Player();

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectImguiDraw()override;
	void Uninit() override;

	void OnCollisionEnter(ComponentBase* _oher) override;
	void OnCollisionStay(ComponentBase* _oher) override;
	void OnCollisionExit(ComponentBase* _oher) override;
};