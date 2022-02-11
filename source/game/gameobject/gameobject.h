#pragma once
#include	"../../system/util/dixsmartptr.h"
#include	"gameobject_utility.h"
#include	"transform.h"
#include	"../component/component_base.h"
#include	"../../system/imgui/util/myimgui.h"
#include	"../manager/scene_manager.h"

#include	<string>
#include	<vector>

class ComponentBase;

// �Q�[���I�u�W�F�N�g�̊��N���X
class GameObject
{
protected:
	std::string			mName;					// ���O
	ObjectState			mObjectState;			// ���
	bool				mIsActive;				// �A�N�e�B�u��(Update��Draw���~�܂�)
	unsigned int		mObjectID;				// �I�u�W�F�N�gID�ԍ�
	ObjectType			mObjectType;			// �I�u�W�F�N�g�^�C�v
	Dix::sp<Transform>	mTransform;				// �ʒu��]�傫��
	bool				mIsStopPause;			// �|�[�Y���Ɏ~�߂邩
	GameObject*			mParent = nullptr;		// �e�I�u�W�F�N�g
	std::string			mSceneKey;

	std::vector<ComponentBase*> mComponentList; // �ێ��R���|�[�l���g���X�g

public:
	GameObject(std::string mName, ObjectType mObjectType, bool isStopPose);
	virtual ~GameObject();

	void Init();
	void Update();
	void ImguiDraw();
	virtual void ImguiCreateDraw();
	virtual void Uninit() {};

	virtual void ObjectInit() {};
	virtual void ObjectUpdate() {};
	virtual void ObjectImguiDraw() {};

	virtual void OnCollisionEnter(ComponentBase* _oher) {};
	virtual void OnCollisionStay(ComponentBase* _oher) {};
	virtual void OnCollisionExit(ComponentBase* _oher) {};

	std::string GetName() const { return mName; }
	void SetName(std::string newName) { mName = newName; }

	void SetObjectType(ObjectType newType) { mObjectType = newType; }
	ObjectType GetObjectType() { return mObjectType; }

	void SetObjectState(ObjectState _state) { mObjectState = _state; }
	ObjectState GetObjectState() { return mObjectState; }

	int GetObjectID() { return mObjectID; }
	bool GetIsStopPause() { return mIsStopPause; }

	void SetIsActive(bool _isActive) { mIsActive = _isActive; }
	bool GetIsActive() { return mIsActive; }

	void SetParent(GameObject* _obj) { mParent = _obj; }
	GameObject* GetParent() { return  mParent; }

	std::string GetScenekey() { return  mSceneKey; }

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