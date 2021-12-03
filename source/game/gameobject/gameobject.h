#pragma once
#include	<string>
#include	<vector>
#include	"gameobject_utility.h"
#include	"transform.h"
#include	"../component/component_base.h"
#include	"../../system/imgui/util/myimgui.h"
#include	"../manager/SceneManager.h"

class ComponentBase;

// �Q�[���I�u�W�F�N�g�̊��N���X
class GameObject
{
protected:
	Transform		mTransform;		// �ʒu��]�傫��
	std::string		mName;			// ���O
	unsigned int	mObjectID;		// �I�u�W�F�N�gID�ԍ�
	ObjectType		mObjectType;	// �I�u�W�F�N�g�^�C�v
	bool			mIsExist;		// ������
	ObjectState		mObjectState;	// ���;

	std::vector<ComponentBase*> componentList;

public:
	GameObject() :mName("NoName"), mObjectType(ObjectType::Obstracle)
	{
		SceneManager::GetInstance()->GetAddScene()->AddGameObject(this);
	}
	GameObject(std::string mName, ObjectType mObjectType) :mName(mName), mObjectType(mObjectType)
	{
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

	Transform* GetTransform() { return &mTransform; }

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

// �R���|�[�l���g��ǉ�����
// AddComponent<�ǉ��������R���|�[�l���g�̃N���X��>()�Ƃ����`�Ŏg��
template<class T>
T* GameObject::AddComponent()
{
	T* newComponent = new T();	// new�����̂�delete��Y�ꂸ�Ɂi�f�X�g���N�^�ł���Ă�j
	if (dynamic_cast<ComponentBase*>(newComponent) != nullptr)
	{
		newComponent->SetOwner(this);
		componentList.emplace_back(newComponent);
	}

	return newComponent;
}

// �R���|�[�l���g���擾����
// GetComponent<�R���|�[�l���g�̃N���X��>()->�R���|�[�l���g�̃��\�b�h()�Ƃ����`�Ŏg��
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
	return nullptr;	// ���Y�R���|�[�l���g���Ȃ����nullptr��return
}

// �R���|�[�l���g���폜����B�g�����͏�L��ɏ�����B
template<class T>
void GameObject::RemoveComponent()
{
	for (unsigned int i = 0; i < componentList.size(); i++) // �폜�͂�������for�����₷��
	{
		T* toRemove = dynamic_cast<T*>(componentList[i]);
		if (toRemove != nullptr)
		{
			componentList.erase(componentList.begin() + i);		// �y�ɍ��Ƃ����Ȃ邪�A���X�g�̗v�f�����������erase�͏d���̂ŁA�ʂ̕��@���g���������ǂ��B����΁I�i�u�������j
			// componentList.shrink_to_fit();					// ���X�g��capasity��size�̕s��v���C�ɂȂ�Ȃ炱������Ă������B�������ق�̏����d���Ȃ�B

			return;// ��邱�Ƃ͏I������̂�return
		}
	}
}