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

// ゲームオブジェクトの基底クラス
class GameObject
{
protected:
	GameObject*				mParent;
	Dix::sp<Transform>		mTransform;		// 位置回転大きさ
	std::string				mName;			// 名前
	unsigned int			mObjectID;		// オブジェクトID番号
	ObjectType				mObjectType;	// オブジェクトタイプ
	bool					mIsExist;		// 生存可否
	ObjectState				mObjectState;	// 状態
	bool					mIsStopPause;		// ポーズ時に止めるか

	std::vector<ComponentBase*> mComponentList;

public:
	GameObject(std::string mName, ObjectType mObjectType, bool isStopPose);
	virtual ~GameObject();

	void Init();
	void Update();
	void Draw();

	virtual void ImguiDraw();
	virtual void ObjectInit() {};
	virtual void ObjectUpdate() {};
	virtual void ObjectDraw() {};
	virtual void ObjectImguiDraw() {};
	virtual void Uninit() {};

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

	int GetObjectID() { return mObjectID; }
	bool GetIsStopPause() { return mIsStopPause; }

	Dix::wp<Transform> GetTransform() { return mTransform; }

	//コンポーネントシステム関係
	// 明示的なインスタンス化
	template<class T>
	T* AddComponent();

	template<class T>
	T* GetComponent();

	template<class T>
	void RemoveComponent();
};

//----------------------------------------------------
//			コンポーネント関係
//----------------------------------------------------

// コンポーネント追加
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

// コンポーネント取得
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
	return nullptr;	// 当該コンポーネントがなければnullptrをreturn
}

// コンポーネント削除
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