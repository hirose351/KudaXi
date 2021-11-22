#pragma once
#include	"../../system/util/uncopyable.h"

class GameManager : public Uncopyable
{
private:
	int mStepCnt;
	bool mIsClear;
	bool mIsOver;

public:
	static GameManager& GetInstance() {
		static GameManager Instance;
		return Instance;
	}

	bool GetIsClear() { return mIsClear; }
	bool GetIsOver() { return mIsOver; }
	void RemoveStepCnt() { mStepCnt--; }
	void ClearCheck()
	{
		if (mStepCnt) {}
	}
};

