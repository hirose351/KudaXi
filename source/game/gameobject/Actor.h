#pragma once
#include "../../system/dx11/util/dx11mathutil.h"
#include <vector>

class Actor
{
public:
	// アクターの状態管理用 
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	// コンストラクターとデストラクター 
	Actor(class Game* game);
	virtual ~Actor() = 0;

	// ゲームから呼び出される更新関数(オーバーライド不可)
	void Update(float deltaTime);

	// アクターが持つ全コンポーネントを更新(オーバーライド不可)
	void UpdateComponents(float deltaTime);

	// アクター独自の更新処理(オーバーライド可能)
	virtual void UpdateActor(float deltaTime);

	// ゲッター / セッター
	const XMFLOAT3& GetPosition() const { return m_Position; }
	void SetPosition(const XMFLOAT3& pos) { m_Position = pos; }

	float GetScale() const { return m_Scale; }
	void SetScale(float scale) { m_Scale = scale; }

	float GetRotation() const { return m_Rotation; }
	void SetRotation(float rotation) { m_Rotation = rotation; }

	State GetState() const { return m_State; }
	void SetState(State state) { m_State = state; }

	class Game* GetGame() { return m_Game; }

	// コンポーネントの追加/ 削除 
	void AddComponent(class IComponent* component);
	void RemoveComponent(class IComponent* component);

private:
	State m_State;					// アクターの状態 

	// 座標変換 
	XMFLOAT3 m_Position; 			// アクターの中心位置 
	float m_Scale; 					// アクターのスケール（1.0fが100%）
	float m_Rotation;				// 回転の角度（ラジアン）

	// アクターが持つコンポーネント
	std::vector<class IComponent*> m_Components;
	class Game* m_Game;
};

