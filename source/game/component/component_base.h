#pragma once
#include "../gameobject/gameobject.h"

class GameObject;

class ComponentBase
{
protected:
	GameObject* mOwner;		// 自身を所有しているゲームオブジェクト
	std::string	mName;		// 名前
	ObjectTag mTag;
	bool mIsActive;
	bool mIsCreate = false;
public:
	ComponentBase(std::string mName) :mOwner(nullptr), mIsActive(true), mName(mName) {}
	ComponentBase(GameObject* mOwner) :mOwner(mOwner) {}
	virtual ~ComponentBase() { Uninit(); mOwner = nullptr; }

	virtual void Awake() {}
	virtual void Init() {}
	virtual void Update() {}
	virtual void ImguiDraw() {}
	virtual void Uninit() {}

	GameObject* GetOwner() { return mOwner; }
	void SetOwner(GameObject* newowner) { mOwner = newowner; }

	ObjectTag GetTag() { return mTag; }
	void SetTag(ObjectTag _tag) { mTag = _tag; }

	std::string GetName() { return mName; }
	void SetName(std::string _name) { mName = _name; }

	bool GetIsActiveWithOwner();
	void SetIsActive(bool _flg) { mIsActive = _flg; }
	bool GetIsActive() { return mIsActive; }

	void SetIsCreate(bool _flg) { mIsCreate = _flg; }
	bool GetIsCreate() { return mIsCreate; }
};

