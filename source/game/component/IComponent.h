#pragma once

class IComponent
{
public:
	// �R���X�g���N�^
	IComponent(class Actor* _owner, const int &_updateOder = 100)
		: m_owner(_owner), m_updateOrder(_updateOder) {}

	// �f�X�g���N�^
	virtual ~IComponent() = 0;

	// ���̃R���|�[�l���g���f���^�^�C���ōX�V����
	virtual void Update(float deltaTime) = 0;

	int GetUpdateOrder() const { return m_updateOrder; }
protected:
	// Owning actor
	class Actor* m_owner;		// ���L�A�N�^�[	
	int m_updateOrder;			// �R���|�[�l���g�̍X�V����
};

