#pragma once

class IComponent
{
public:
	// コンストラクタ
	IComponent(class Actor* _owner, const int &_updateOder = 100)
		: m_owner(_owner), m_updateOrder(_updateOder) {}

	// デストラクタ
	virtual ~IComponent() = 0;

	// このコンポーネントをデルタタイムで更新する
	virtual void Update(float deltaTime) = 0;

	int GetUpdateOrder() const { return m_updateOrder; }
protected:
	// Owning actor
	class Actor* m_owner;		// 所有アクター	
	int m_updateOrder;			// コンポーネントの更新順序
};

