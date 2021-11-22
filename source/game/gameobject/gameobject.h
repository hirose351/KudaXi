#pragma once
#include	<string>
#include	"gameobject_utility.h"

//ゲームオブジェクトの基底クラス
class GameObject
{
protected:
	std::string		mName;			//名前
	unsigned int	mObjectID;		//オブジェクトID番号
	bool			mIsExist;		//生存可否
	ObjectType		mObjectType;		//オブジェクトタイプ// アクター の 状態 State mState;
	ObjectState		mObjectState;

public:
	GameObject() :mName("NoName"), mObjectType(ObjectType::Obstracle) {}
	virtual ~GameObject();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;

	std::string GetName() const { return mName; }
	void SetName(std::string newName) { mName = newName; }

	bool GetExistState() const { return mIsExist; }
	void SetExistState(bool newState) { mIsExist = newState; }

	void SetObjectType(ObjectType newType) { mObjectType = newType; }
	ObjectType GetObjectType() { return mObjectType; }
};