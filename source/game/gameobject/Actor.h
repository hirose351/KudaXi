#pragma once
#include "../../system/dx11/util/dx11mathutil.h"
#include <vector>

class Actor
{
public:
	// �A�N�^�[�̏�ԊǗ��p 
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	// �R���X�g���N�^�[�ƃf�X�g���N�^�[ 
	Actor(class Game* game);
	virtual ~Actor() = 0;

	// �Q�[������Ăяo�����X�V�֐�(�I�[�o�[���C�h�s��)
	void Update(float deltaTime);

	// �A�N�^�[�����S�R���|�[�l���g���X�V(�I�[�o�[���C�h�s��)
	void UpdateComponents(float deltaTime);

	// �A�N�^�[�Ǝ��̍X�V����(�I�[�o�[���C�h�\)
	virtual void UpdateActor(float deltaTime);

	// �Q�b�^�[ / �Z�b�^�[
	const XMFLOAT3& GetPosition() const { return m_Position; }
	void SetPosition(const XMFLOAT3& pos) { m_Position = pos; }

	float GetScale() const { return m_Scale; }
	void SetScale(float scale) { m_Scale = scale; }

	float GetRotation() const { return m_Rotation; }
	void SetRotation(float rotation) { m_Rotation = rotation; }

	State GetState() const { return m_State; }
	void SetState(State state) { m_State = state; }

	class Game* GetGame() { return m_Game; }

	// �R���|�[�l���g�̒ǉ�/ �폜 
	void AddComponent(class IComponent* component);
	void RemoveComponent(class IComponent* component);

private:
	State m_State;					// �A�N�^�[�̏�� 

	// ���W�ϊ� 
	XMFLOAT3 m_Position; 			// �A�N�^�[�̒��S�ʒu 
	float m_Scale; 					// �A�N�^�[�̃X�P�[���i1.0f��100%�j
	float m_Rotation;				// ��]�̊p�x�i���W�A���j

	// �A�N�^�[�����R���|�[�l���g
	std::vector<class IComponent*> m_Components;
	class Game* m_Game;
};

