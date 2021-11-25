#pragma once
#include	"../../system/util/uncopyable.h"
#include	"../../system/dx11/dx11mathutil.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../gameobject/primitive_utility.h"
#include	<list>

struct PrimitiveBase;

class CollisionManager : public Uncopyable
{
private:
	std::list<Dix::sp<struct PrimitiveBase>> mPrimList;

public:
	static CollisionManager& GetInstance() {
		static CollisionManager Instance;
		return Instance;
	}
	~CollisionManager() { mPrimList.clear(); }

	// �ǉ�
	void AddPrim(PrimitiveBase *_col);
	// �ǉ�
	void RemovePrim(PrimitiveBase *_col);

	// �X�V
	void UpdateCollision();
};

