#pragma once
#include	"IGameManager.h"

class SelectManager :public IGameManager
{
private:
public:
	// ������
	void Init() override;

	// �X�V
	void Update()override;

	// �`��
	void Draw() override;

	// �j��
	void Dispose() override;
};