#pragma once
#include "../../system/util/uncopyable.h"

class IGameManager :public Uncopyable
{
protected:
public:

	virtual ~IGameManager() {}
	// ������
	virtual void Init() = 0;
	// �X�V
	virtual void Update() = 0;
	// �`��
	virtual void Draw() = 0;
	// �j��
	virtual void Dispose() = 0;
};

