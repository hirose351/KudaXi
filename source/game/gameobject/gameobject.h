#pragma once
#include	<string>
#include	<vector>
#include	"gameobject_utility.h"
#include	"transform.h"
#include	"../component/component_base.h"
#include	"../../system/imgui/util/myimgui.h"
#include	"../manager/SceneManager.h"
#include	"../../system/util/dixsmartptr.h"

class ComponentBase;

// �Q�[���I�u�W�F�N�g�̊��N���X
class GameObject
{
protected:
	GameObject*				mParent;
	Dix::sp<Transform>		mTransform;		// �ʒu��]�傫��
	std::string				mName;			// ���O
	unsigned int			mObjectID;		// �I�u�W�F�N�gID�ԍ�
	ObjectType				mObjectType;	// �I�u�W�F�N�g�^�C�v
	bool					mIsExist;		// ������
	ObjectState				mObjectState;	// ���

	std::vector<ComponentBase*> mComponentList;

public:
	GameObject() :mName("NoName"), mObjectType(ObjectType::eObstracle)
	{
		mTransform.SetPtr(new Transform);
		SceneManager::GetInstance()->GetAddScene()->AddGameObject(this);
	}
	GameObject(std::string mName, ObjectType mObjectType) :mName(mName), mObjectType(mObjectType)
	{
		mTransform.SetPtr(new Transform);
		SceneManager::GetInstance()->GetAddScene()->AddGameObject(this);
	}
	virtual ~GameObject();

	void Init();
	void Update();
	void Draw();

	virtual void ImguiDraw();
	virtual void ObjectInit() = 0;
	virtual void ObjectUpdate() = 0;
	virtual void ObjectDraw() = 0;
	virtual void ObjectImguiDraw() = 0;
	virtual void Uninit() = 0;

	virtual void OnCollisionEnter(ComponentBase* _oher) {};
	virtual void OnCollisionStay(ComponentBase* _oher) {};
	virtual void OnCollisionExit(ComponentBase* _oher) {};

	std::string GetName() const { return mName; }
	void SetName(std::string newName) { mName = newName; }

	bool GetExistState() const { return mIsExist; }
	void SetExistState(bool newState) { mIsExist = newState; }

	void SetObjectType(ObjectType newType) { mObjectType = newType; }
	ObjectType GetObjectType() { return mObjectType; }

	void SetObjectState(ObjectState _state) { mObjectState = _state; }
	ObjectState GetObjectState() { return mObjectState; }

	Dix::wp<Transform> GetTransform() { return mTransform; }

	//�R���|�[�l���g�V�X�e���֌W
	// �����I�ȃC���X�^���X��
	template<class T>
	T* AddComponent();

	template<class T>
	T* GetComponent();

	template<class T>
	void RemoveComponent();
};

//----------------------------------------------------
//			�R���|�[�l���g�֌W
//----------------------------------------------------

// �R���|�[�l���g�ǉ�
template<class T>
T* GameObject::AddComponent()
{
	T* newComponent = new T();
	if (dynamic_cast<ComponentBase*>(newComponent) != nullptr)
	{
		newComponent->SetOwner(this);
		mComponentList.emplace_back(newComponent);
	}

	return newComponent;
}

// �R���|�[�l���g�擾
template<class T>
T* GameObject::GetComponent()
{
	for (auto &com : mComponentList)
	{
		T* sp = dynamic_cast<T*>(com);
		if (sp != NULL)
		{
			return sp;
		}
	}
	return nullptr;	// ���Y�R���|�[�l���g���Ȃ����nullptr��return
}

// �R���|�[�l���g�폜
template<class T>
void GameObject::RemoveComponent()
{
	for (unsigned int i = 0; i < mComponentList.size(); i++)
	{
		T* toRemove = dynamic_cast<T*>(mComponentList[i]);
		if (toRemove != nullptr)
		{
			mComponentList.erase(mComponentList.begin() + i);
			mComponentList.shrink_to_fit();
			return;
		}
	}
}