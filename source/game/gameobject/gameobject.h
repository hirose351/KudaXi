#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "../component/component_base.h"
//#include "../component/allcomponents.h"
class ComponentBase;

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

	std::vector<ComponentBase*> componentList;

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


	//�R���|�[�l���g�V�X�e���֌W
	//�e���v���[�g�ɂ��Ă킩��Ȃ���Β��ׂĂ�
	// �����I�ȃC���X�^���X��
	template<class T>
	T* AddComponent();

	template<class T>
	T* GetComponent();

	template<class T>
	void RemoveComponent();

	//template ComponentBase* GameObject::AddComponent<ComponentBase>();
};


//----------------------------------------------------
//			�R���|�[�l���g�֌W
//----------------------------------------------------

//�R���|�[�l���g��ǉ�����
//AddComponent<�ǉ��������R���|�[�l���g�̃N���X��>()�Ƃ����`�Ŏg��
template<class T>
T* GameObject::AddComponent()
{
	T* newComponent = new T();//new�����̂�delete��Y�ꂸ�Ɂi�f�X�g���N�^�ł���Ă�j
	if (dynamic_cast<ComponentBase*>(newComponent) != nullptr)
	{
		newComponent->SetOwner(this);
		componentList.emplace_back(newComponent);
	}

	return newComponent;
}

//�R���|�[�l���g���擾����
//GetComponent<�R���|�[�l���g�̃N���X��>()->�R���|�[�l���g�̃��\�b�h()�Ƃ����`�Ŏg��
template<class T>
T* GameObject::GetComponent()
{
	for (auto &com : componentList)
	{
		T* sp = dynamic_cast<T*>(com);
		if (sp != NULL)
		{
			return sp;
		}
	}

	return nullptr;//���Y�R���|�[�l���g���Ȃ����nullptr��return
}

//�R���|�[�l���g���폜����B�g�����͏�L��ɏ�����B
template<class T>
void GameObject::RemoveComponent()
{
	for (unsigned int i = 0; i < componentList.size(); i++) //�폜�͂�������for�����₷��
	{
		T* toRemove = dynamic_cast<T*>(componentList[i]);
		if (toRemove != nullptr)
		{
			componentList.erase(componentList.begin() + i);		//�y�ɍ��Ƃ����Ȃ邪�A���X�g�̗v�f�����������erase�͏d���̂ŁA�ʂ̕��@���g���������ǂ��B����΁I�i�u�������j
			//componentList.shrink_to_fit();					//���X�g��capasity��size�̕s��v���C�ɂȂ�Ȃ炱������Ă������B�������ق�̏����d���Ȃ�B

			return;//��邱�Ƃ͏I������̂�return
		}
	}
}