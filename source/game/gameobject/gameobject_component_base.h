#pragma once
//#include <iostream>
//#include <string>
//#include <vector>
//
//#include "../component/ComponentBase.h"
//#include "../gameobject/gameobject.h"
//
//class ComponentBace;
//
////�I�u�W�F�N�g�^�C�v
//enum class ObjectType
//{
//	Player = 0b00,
//	Enemy = 0b01,
//	Obstracle = 0b11,
//};
//
////�Q�[���I�u�W�F�N�g�̊��N���X
//class GameObjectComponent :public ComponentBace
//{
//protected:
//	GameObject*
//
//
//	std::string		name;			//���O
//	unsigned int	objectID;		//�I�u�W�F�N�gID�ԍ�
//	bool			isExist;		//������
//	ObjectType		objectType;		//�I�u�W�F�N�g�^�C�v
//
//	std::vector<ComponentBace*> componentList;
//
//public:
//	enum State
//	{
//		EActive,
//		EPaused,
//		EDead
//	};
//
//	GameObject() :name("NoName"), objectType(ObjectType::Obstracle) {}
//	virtual ~GameObject();
//
//	std::string GetName() const { return name; }
//	void SetName(std::string newName) { name = newName; }
//
//	bool GetExistState() const { return isExist; }
//	void SetExistState(bool newState) { isExist = newState; }
//
//	void SetObjectType(ObjectType newType) { objectType = newType; }
//	ObjectType GetObjectType() { return objectType; }
//
//	virtual void Update();											//�I�[�o�[���C�h���Ďg�u����v���\�b�h�B�I�u�W�F�N�g���Ƃɏ��������ĂˁB��{�̓I�[�o�[���C�h���Ȃ��Ă����B
//	virtual void CollisionResponse(GameObject* opponent) = 0;		//����͂ǂ��ɂ����Ă�
//
//	void Draw();
//
//
//	//�R���|�[�l���g�V�X�e���֌W
//	//�e���v���[�g�ɂ��Ă킩��Ȃ���Β��ׂĂ�
//	template<class T>
//	T* AddComponent();
//
//	template<class T>
//	T* GetComponent();
//
//	template<class T>
//	void RemoveComponent();
//};