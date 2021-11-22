#pragma once
#include	<string>
#include	"gameobject_utility.h"

//�Q�[���I�u�W�F�N�g�̊��N���X
class GameObject
{
protected:
	std::string		mName;			//���O
	unsigned int	mObjectID;		//�I�u�W�F�N�gID�ԍ�
	bool			mIsExist;		//������
	ObjectType		mObjectType;		//�I�u�W�F�N�g�^�C�v// �A�N�^�[ �� ��� State mState;
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