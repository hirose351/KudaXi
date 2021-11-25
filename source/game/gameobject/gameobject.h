#pragma once
#include	<string>
#include	"gameobject_utility.h"
#include	"transform.h"

//�Q�[���I�u�W�F�N�g�̊��N���X
class GameObject
{
protected:
	Transform		mTransform;		// �ʒu��]�傫��
	std::string		mName;			// ���O
	unsigned int	mObjectID;		// �I�u�W�F�N�gID�ԍ�
	ObjectType		mObjectType;	// �I�u�W�F�N�g�^�C�v
	bool			mIsExist;		// ������
	ObjectState		mObjectState;	// ���;

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