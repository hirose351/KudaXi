#include "Actor.h"
#include "../../game.h"
#include "../component/IComponent.h"

Actor::Actor(Game * game)
	: m_State(EActive)
	, m_Position(XMFLOAT3(0, 0, 0))
	, m_Scale(1.0f)
	, m_Rotation(0.0f)
	, m_Game(game)
{
	m_Game->AddActor(this);
}

Actor::~Actor()
{
	m_Game->RemoveActor(this);
	// コンポーネントを削除
	// ~ComponentでRemoveComponentが呼び出されるので、別の種類のループを使う
	while (!m_Components.empty())
	{
		delete m_Components.back();
	}
}

void Actor::Update(float deltaTime)
{
	if (m_State == EActive)
	{
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : m_Components)
	{
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime)
{
}

void Actor::AddComponent(IComponent* component)
{
	// ソートされたベクトルで挿入ポイントを見つける
	// 私より高次の最初の要素
	int myOrder = component->GetUpdateOrder();
	auto iter = m_Components.begin();
	for (;
		 iter != m_Components.end();
		 ++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	// イテレータの位置の前に要素を挿入します
	m_Components.insert(iter, component);
}

void Actor::RemoveComponent(IComponent* component)
{
}
