#pragma once
#include <iostream>
#include <string>
#include <vector>

//�I�u�W�F�N�g�^�C�v
enum class ObjectType
{
	Player = 0b00,
	Enemy = 0b01,
	Obstracle = 0b11,
};

//�Q�[���I�u�W�F�N�g�̊��N���X
class GameObject
{
protected:
	/*	�S�Q�[���I�u�W�F�N�g�������Ă�p�����[�^�������ɓ˂�����ł����B
		�O����G��Ȃ��悤�ɂ��Ă����ƁA�s�ӂɒl���ς�邱�Ƃ�����A�Ǘ����₷���B	*/

	std::string		name;			//���O
	unsigned int	objectID;		//�I�u�W�F�N�gID�ԍ�
	bool			isExist;		//������
	ObjectType		objectType;		//�I�u�W�F�N�g�^�C�v

	//std::vector<ComponentBase*> componentList;

public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	GameObject() :name("NoName"), objectType(ObjectType::Obstracle) {}
	virtual ~GameObject();

	std::string GetName() const { return name; }
	void SetName(std::string newName) { name = newName; }

	bool GetExistState() const { return isExist; }
	void SetExistState(bool newState) { isExist = newState; }

	void SetObjectType(ObjectType newType) { objectType = newType; }
	ObjectType GetObjectType() { return objectType; }

	virtual void Update();											//�I�[�o�[���C�h���Ďg�u����v���\�b�h�B�I�u�W�F�N�g���Ƃɏ��������ĂˁB��{�̓I�[�o�[���C�h���Ȃ��Ă����B
	virtual void CollisionResponse(GameObject* opponent) = 0;		//����͂ǂ��ɂ����Ă�

	void Draw();
};