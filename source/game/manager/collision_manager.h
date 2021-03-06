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

	// 追加
	void AddCollision(Component::Collision *_col);
	// 削除
	void RemoveCollision(Component::Collision *_col);

	// 更新
	void Update();
};

