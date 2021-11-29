#include "collision_manager.h"

//using namespace Component;

void CollisionManager::AddCollision(Component::CollisionComponent *_col)
{
	mColList.emplace_back(_col);
}

void CollisionManager::RemoveCollision(Component::CollisionComponent *_col)
{
	//mPrimList.erase(_col);
}

void CollisionManager::Update()
{
	for (auto itA = mColList.begin(); itA != mColList.end(); itA++)
	{
		for (auto itB = itA; itB != mColList.end(); itB++)
		{
			if (itA == itB) continue;
			Primitive::AABB a = (*itA)->GetPrim();
			Primitive::AABB b = (*itB)->GetPrim();

			if ((a.hl.x + b.hl.x) < std::abs(a.p.x - b.p.x)) continue;
			if ((a.hl.y + b.hl.y) < std::abs(a.p.y - b.p.x)) continue;
			if ((a.hl.z + b.hl.z) < std::abs(a.p.z - b.p.z)) continue;



			//	(*itA)->OnCollision((*itB));
			//	(*itB)->OnCollision((*itA));
		}
	}
	mColList.clear();
}
