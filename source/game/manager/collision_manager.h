#pragma once
#include	"../../system/util/uncopyable.h"
#include	"../../system/dx11/dx11mathutil.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../gameobject/collision_base.h"
#include	<list>

class CollisionManager : public Uncopyable
{
private:
	//std::list<Dix::sp<class CollisionBase>> mColList;

public:
	static CollisionManager& GetInstance() {
		static CollisionManager Instance;
		return Instance;
	}

	// í«â¡
	void AddCollision(const CollisionBase &_col);

	// çXêV
	void UpdateCollision();
};

