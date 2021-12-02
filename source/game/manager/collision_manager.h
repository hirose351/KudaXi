#pragma once
#include	"../../system/util/uncopyable.h"
#include	"../../system/dx11/dx11mathutil.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../component/collision_component.h"

class Component::CollisionComponent;

class CollisionManager : public Uncopyable
{
private:
	std::vector<Dix::sp<class Component::CollisionComponent>> mColList;

public:
	static CollisionManager& GetInstance() {
		static CollisionManager Instance;
		return Instance;
	}
	~CollisionManager() {}

	// �ǉ�
	void AddCollision(Component::CollisionComponent *_col);
	// �폜
	void RemoveCollision(Component::CollisionComponent *_col);

	// �X�V
	void Update();
};

