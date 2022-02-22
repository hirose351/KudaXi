#pragma once
#include	"../../system/util/uncopyable.h"
#include	"../component/collision_component.h"

class Component::Collision;
class CollisionManager : public Uncopyable
{
private:
	std::vector<class Component::Collision*> mpColList;

public:
	static CollisionManager& GetInstance() {
		static CollisionManager Instance;
		return Instance;
	}
	~CollisionManager();

	// ’Ç‰Á
	void AddCollision(Component::Collision *_col);
	// íœ
	void RemoveCollision(Component::Collision *_col);

	// XV
	void Update();
};

