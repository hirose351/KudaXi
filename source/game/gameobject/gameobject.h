#pragma once
#include	<string>
#include	"gameobject_utility.h"
#include	"transform.h"

//ゲームオブジェクトの基底クラス
class GameObject
{
protected:
	Transform		mTransform;		// 位置回転大きさ
	std::string		mName;			// 名前
	unsigned int	mObjectID;		// オブジェクトID番号
	ObjectType		mObjectType;	// オブジェクトタイプ
	bool			mIsExist;		// 生存可否
	ObjectState		mObjectState;	// 状態;

public:
	GameObject() :mName("NoName"), mObjectType(ObjectType::Obstracle) {}
	GameObject(std::string mName, ObjectType mObjectType) :mName(mName), mObjectType(mObjectType) {}
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