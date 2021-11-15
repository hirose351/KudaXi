#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "../component/component_base.h"
//#include "../component/allcomponents.h"
class ComponentBase;

//オブジェクトタイプ
enum class ObjectType
{
	Player = 0b00,
	Enemy = 0b01,
	Obstracle = 0b11,
};

//ゲームオブジェクトの基底クラス
class GameObject
{
protected:
	/*	全ゲームオブジェクトが持ってるパラメータをここに突っ込んでいく。
		外から触らないようにしておくと、不意に値が変わることが減り、管理しやすい。	*/

	std::string		name;			//名前
	unsigned int	objectID;		//オブジェクトID番号
	bool			isExist;		//生存可否
	ObjectType		objectType;		//オブジェクトタイプ

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

	virtual void Update();											//オーバーライドして使「える」メソッド。オブジェクトごとに書き分けてね。基本はオーバーライドしなくていい。
	virtual void CollisionResponse(GameObject* opponent) = 0;		//これはどうにかしてね

	void Draw();


	//コンポーネントシステム関係
	//テンプレートについてわからなければ調べてね
	// 明示的なインスタンス化
	template<class T>
	T* AddComponent();

	template<class T>
	T* GetComponent();

	template<class T>
	void RemoveComponent();

	//template ComponentBase* GameObject::AddComponent<ComponentBase>();
};


//----------------------------------------------------
//			コンポーネント関係
//----------------------------------------------------

//コンポーネントを追加する
//AddComponent<追加したいコンポーネントのクラス名>()という形で使う
template<class T>
T* GameObject::AddComponent()
{
	T* newComponent = new T();//newしたのでdeleteを忘れずに（デストラクタでやってる）
	if (dynamic_cast<ComponentBase*>(newComponent) != nullptr)
	{
		newComponent->SetOwner(this);
		componentList.emplace_back(newComponent);
	}

	return newComponent;
}

//コンポーネントを取得する
//GetComponent<コンポーネントのクラス名>()->コンポーネントのメソッド()という形で使う
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

	return nullptr;//当該コンポーネントがなければnullptrをreturn
}

//コンポーネントを削除する。使い方は上記二つに準ずる。
template<class T>
void GameObject::RemoveComponent()
{
	for (unsigned int i = 0; i < componentList.size(); i++) //削除はこっちのforが作りやすい
	{
		T* toRemove = dynamic_cast<T*>(componentList[i]);
		if (toRemove != nullptr)
		{
			componentList.erase(componentList.begin() + i);		//楽に作るとこうなるが、リストの要素数が増えるとeraseは重いので、別の方法を使った方が良い。がんば！（ブン投げ）
			//componentList.shrink_to_fit();					//リストのcapasityとsizeの不一致が気になるならこれを入れてもいい。ただしほんの少し重くなる。

			return;//やることは終わったのでreturn
		}
	}
}