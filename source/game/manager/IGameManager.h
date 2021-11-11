#pragma once
#include "../../system/util/uncopyable.h"

class IGameManager :public Uncopyable
{
protected:
public:

	virtual ~IGameManager() {}
	// ‰Šú‰»
	virtual void Init() = 0;
	// XV
	virtual void Update() = 0;
	// •`‰æ
	virtual void Draw() = 0;
	// ”jŠü
	virtual void Dispose() = 0;
};

