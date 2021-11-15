#pragma once
#include "../gameobject/gameobject.h"

class GameObject;

class ComponentBase
{
protected:
	GameObject* mOwner;		// 自身を所有しているゲームオブジェクト

public:
	ComponentBase() { mOwner = nullptr; }
	virtual ~ComponentBase() { mOwner = nullptr; }

	GameObject* GetOwner() { return mOwner; }
	GameObject* SetOwner(GameObject* newowner) { mOwner = newowner;  return mOwner; }

	virtual void Draw() {}		//描画ナシで更新だけするコンポーネントが生まれる（かもしれない）ので以下略（同じくオーバーヘッドが発生しそうだが、コンパイラがどうにかしてくれる）
	virtual void Update() {}	//更新ナシで描画だけするコンポーネントが生まれる（かもしれない）ので、純粋仮想関数にはしないでおく（一見オーバーヘッドが発生しそうだが、コンパイラがどうにかしてくれる）
};