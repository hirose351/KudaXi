#pragma once
#include "../gameobject/gameobject.h"

class GameObject;

class ComponentBase
{
protected:
	GameObject* mOwner;		// 自身を所有しているゲームオブジェクト
	ObjectTag mTag;

public:
	ComponentBase() { mOwner = nullptr; }
	ComponentBase(GameObject* mOwner) :mOwner(mOwner) {}
	virtual ~ComponentBase() { mOwner = nullptr; }

	GameObject* GetOwner() { return mOwner; }
	void SetOwner(GameObject* newowner) { mOwner = newowner; }


	ObjectTag GetTag() { return mTag; }
	void SetTag(ObjectTag _tag) { mTag = _tag; }

	virtual void Awake() {}
	virtual void Init() {}
	virtual void Update() {}
	virtual void Draw() {}
	virtual void Uninit() {}
};

