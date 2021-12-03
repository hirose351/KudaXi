#pragma once
#include	<string>
#include	<vector>
#include	"gameobject_utility.h"
#include	"transform.h"
#include	"../component/component_base.h"
#include	"../../system/imgui/util/myimgui.h"
#include	"../manager/SceneManager.h"

class ComponentBase;

// ゲームオブジェクトの基底クラス
class GameObject
{
protected:
	Transform		mTransform;		// 位置回転大きさ
	std::string		mName;			// 名前
	unsigned int	mObjectID;		// オブジェクトID番号
	ObjectType		mObjectType;	// オブジェクトタイプ
	bool			mIsExist;		// 生存可否
	ObjectState		mObjectState;	// 状態;

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

// コンポーネントを追加する
// AddComponent<追加したいコンポーネントのクラス名>()という形で使う
template<class T>
T* GameObject::AddComponent()
{
	T* newComponent = new T();	// newしたのでdeleteを忘れずに（デストラクタでやってる）
	if (dynamic_cast<ComponentBase*>(newComponent) != nullptr)
	{
		newComponent->SetOwner(this);
		componentList.emplace_back(newComponent);
	}

	return newComponent;
}

// コンポーネントを取得する
// GetComponent<コンポーネントのクラス名>()->コンポーネントのメソッド()という形で使う
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
	return nullptr;	// 当該コンポーネントがなければnullptrをreturn
}

// コンポーネントを削除する。使い方は上記二つに準ずる。
template<class T>
void GameObject::RemoveComponent()
{
	for (unsigned int i = 0; i < componentList.size(); i++) // 削除はこっちのforが作りやすい
	{
		T* toRemove = dynamic_cast<T*>(componentList[i]);
		if (toRemove != nullptr)
		{
			componentList.erase(componentList.begin() + i);		// 楽に作るとこうなるが、リストの要素数が増えるとeraseは重いので、別の方法を使った方が良い。がんば！（ブン投げ）
			// componentList.shrink_to_fit();					// リストのcapasityとsizeの不一致が気になるならこれを入れてもいい。ただしほんの少し重くなる。

			return;// やることは終わったのでreturn
		}
	}
}