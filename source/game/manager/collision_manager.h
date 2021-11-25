#pragma once
#include	"../../system/util/uncopyable.h"
#include	"../../system/dx11/dx11mathutil.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../gameobject/primitive_utility.h"
#include	<list>

class PrimitiveBase;

class CollisionManager : public Uncopyable
{
private:
	std::list<Dix::sp<class PrimitiveBase>> mPrimList;

public:
	static CollisionManager& GetInstance() {
		static CollisionManager Instance;
		return Instance;
	}
	~CollisionManager() { mPrimList.clear(); }

	// 追加
	void AddPrim(PrimitiveBase *_col);
	// 追加
	void RemovePrim(PrimitiveBase *_col);

	// 更新
	void UpdateCollision();
};

