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
////オブジェクトタイプ
//enum class ObjectType
//{
//	Player = 0b00,
//	Enemy = 0b01,
//	Obstracle = 0b11,
//};
//
////ゲームオブジェクトの基底クラス
//class GameObjectComponent :public ComponentBace
//{
//protected:
//	GameObject*
//
//
//	std::string		name;			//名前
//	unsigned int	objectID;		//オブジェクトID番号
//	bool			isExist;		//生存可否
//	ObjectType		objectType;		//オブジェクトタイプ
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
//	virtual void Update();											//オーバーライドして使「える」メソッド。オブジェクトごとに書き分けてね。基本はオーバーライドしなくていい。
//	virtual void CollisionResponse(GameObject* opponent) = 0;		//これはどうにかしてね
//
//	void Draw();
//
//
//	//コンポーネントシステム関係
//	//テンプレートについてわからなければ調べてね
//	template<class T>
//	T* AddComponent();
//
//	template<class T>
//	T* GetComponent();
//
//	template<class T>
//	void RemoveComponent();
//};