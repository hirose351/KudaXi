#pragma once
#include "../gameobject/gameobject.h"

class GameObject;

class ComponentBase
{
protected:
	GameObject* mOwner;		// ���g�����L���Ă���Q�[���I�u�W�F�N�g
	ObjectTag mTag;
	bool mIsActive;
public:
	ComponentBase() :mOwner(nullptr), mIsActive(true) {}
	ComponentBase(GameObject* mOwner) :mOwner(mOwner) {}
	virtual ~ComponentBase() { Uninit(); mOwner = nullptr; }

	GameObject* GetOwner() { return mOwner; }
	void SetOwner(GameObject* newowner) { mOwner = newowner; }

	ObjectTag GetTag() { return mTag; }
	void SetTag(ObjectTag _tag) { mTag = _tag; }

	bool GetIsActive() { return mIsActive; }
	void SetIsActive(bool _flg) { mIsActive = _flg; }

	virtual void Awake() {}
	virtual void Init() {}
	virtual void Update() {}
	virtual void Draw() {}
	virtual void ImguiDraw() {}
	virtual void Uninit() {}
};

