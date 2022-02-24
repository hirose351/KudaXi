#pragma once
#include "../gameobject/gameobject.h"

class GameObject;
class ComponentBase
{
protected:
	GameObject* mpOwner = nullptr;		// 自身を所有しているゲームオブジェクト
	std::string	mName;					// 名前
	ObjectTag mTag;
	ObjectState mState;
	bool mIsCreate = false;
	unsigned int mComponentId = 0;

public:
	ComponentBase(std::string mName) : mName(mName), mState(ObjectState::eActive) {}
	ComponentBase(GameObject* mpOwner) :mpOwner(mpOwner) {}
	virtual ~ComponentBase() { Uninit(); mpOwner = nullptr; }

	virtual void Awake() {}
	virtual void Init() {}
	virtual void Update() {}
	virtual void ImguiDraw() {}
	virtual void Uninit() {}

	GameObject* GetOwner() { return mpOwner; }
	void SetOwner(GameObject* newOwner) { mpOwner = newOwner; }

	ObjectTag GetTag() { return mTag; }
	void SetTag(ObjectTag _tag) { mTag = _tag; }

	std::string GetName() { return mName; }
	void SetName(std::string _name) { mName = _name; }

	bool GetIsActiveWithOwner();

	void SetState(ObjectState _state) { mState = _state; }
	ObjectState GetState() { return mState; }

	void SetIsCreate(bool _flg) { mIsCreate = _flg; }
	bool GetIsCreate() { return mIsCreate; }

	void SetComponentId(int _id) { mComponentId = _id; }
	int GetComponentId() { return mComponentId; }
};

