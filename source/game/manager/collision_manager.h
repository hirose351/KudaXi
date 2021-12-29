#pragma once
#include	"../../system/util/uncopyable.h"
#include	"../../system/dx11/dx11mathutil.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../component/collision_component.h"

class Component::Collision;

class CollisionManager : public Uncopyable
{
private:
	std::vector<class Component::Collision*> mColList;

public:
	static CollisionManager& GetInstance() {
		static CollisionManager Instance;
		return Instance;
	}
	~CollisionManager();

	// �ǉ�
	void AddCollision(Component::Collision *_col);
	// �폜
	void RemoveCollision(Component::Collision *_col);

	// �X�V
	void Update();
};

