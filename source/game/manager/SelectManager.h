#pragma once
#include	"IGameManager.h"

class SelectManager :public IGameManager
{
private:
public:
	// ‰Šú‰»
	void Init() override;

	// XV
	void Update()override;

	// •`‰æ
	void Draw() override;

	// ”jŠü
	void Dispose() override;
};