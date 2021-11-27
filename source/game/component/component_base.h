#pragma once
#include "../gameobject/gameobject.h"

class ComponentBase
{
protected:
	GameObject* mOwner;		// 自身を所有しているゲームオブジェクト

public:
	ComponentBase() { mOwner = nullptr; }
	ComponentBase(GameObject* mOwner) :mOwner(mOwner) {}
	virtual ~ComponentBase() { mOwner = nullptr; }

	GameObject* GetOwner() { return mOwner; }
	GameObject* SetOwner(GameObject* newowner) { mOwner = newowner;  return mOwner; }

	virtual void Awake() {}
	virtual void Init() {}
	virtual void Update() {}
	virtual void Draw() {}
	virtual void Uninit() {}
};

