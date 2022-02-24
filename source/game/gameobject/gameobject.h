#pragma once
#include	"gameobject_utility.h"
#include	"transform.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../../system/imgui/util/myimgui.h"
#include	"../component/component_base.h"
#include	"../manager/scene_manager.h"
#include	<string>
#include	<vector>

class ComponentBase;

// ゲームオブジェクトの基底クラス
class GameObject
{
protected:
	std::string			mName;					// 名前
	ObjectState			mObjectState;			// 状態
	bool				mIsActive;				// アクティブか(UpdateもDrawも止まる)
	unsigned int		mObjectID;				// オブジェクトID番号
	ObjectType			mObjectType;			// オブジェクトタイプ
	Dix::sp<Transform>	mTransform;				// 位置回転大きさ
	bool				mIsStopPause;			// ポーズ時に止めるか
	GameObject*			mpParent = nullptr;		// 親オブジェクト
	std::string			mSceneKey;				// 存在しているシーン名

	std::vector<ComponentBase*> mpComponentList; // 保持コンポーネントリスト

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
	void SetIsStopPause(bool _flg) { mIsStopPause = _flg; }

	void SetIsActive(bool _isActive) { mIsActive = _isActive; }
	bool GetIsActive() { return mIsActive; }

	void SetParent(GameObject* _obj) { mpParent = _obj; }
	GameObject* GetParent() { return  mpParent; }

	std::string GetScenekey() { return  mSceneKey; }

	Dix::wp<Transform> GetTransform() { return mTransform; }

	//コンポーネントシステム関係
	// 明示的なインスタンス化
	template<class T>
	T* AddComponent();

	template<class T>
	T* GetComponent();

	template<class T>
	T* GetComponents(int _num);

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
	// 同じ型のコンポーネントがあれば加算
	int componentCnt = 0;
	for (auto &com : mpComponentList)
	{
		T* sp = dynamic_cast<T*>(com);
		if (sp != NULL)
		{
			componentCnt++;
		}
	}

	T* newComponent = new T();
	if (dynamic_cast<ComponentBase*>(newComponent) != nullptr)
	{
		newComponent->SetOwner(this);
		newComponent->SetComponentId(componentCnt);
		mpComponentList.emplace_back(newComponent);
	}

	return newComponent;
}

// コンポーネント取得
template<class T>
T* GameObject::GetComponent()
{
	for (auto &com : mpComponentList)
	{
		T* sp = dynamic_cast<T*>(com);
		if (sp != NULL)
		{
			return sp;
		}
	}
	return nullptr;	// 当該コンポーネントがなければnullptrをreturn
}

// コンポーネント取得
template<class T>
T* GameObject::GetComponents(int _num)
{
	int numCnt = 0;
	for (auto &com : mpComponentList)
	{
		T* sp = dynamic_cast<T*>(com);
		if (sp != NULL)
		{
			if (numCnt == _num)
				return sp;
			numCnt++;
		}
	}
	return nullptr;	// 当該コンポーネントがなければnullptrをreturn
}

// コンポーネント削除
template<class T>
void GameObject::RemoveComponent()
{
	for (unsigned int i = 0; i < mpComponentList.size(); i++)
	{
		T* toRemove = dynamic_cast<T*>(mpComponentList[i]);
		if (toRemove != nullptr)
		{
			mpComponentList.erase(mpComponentList.begin() + i);
			mpComponentList.shrink_to_fit();
			return;
		}
	}
}