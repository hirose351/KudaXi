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

	// í«â¡
	void AddCollision(Component::Collision *_col);
	// çÌèú
	void RemoveCollision(Component::Collision *_col);

	// çXêV
	void Update();
};

