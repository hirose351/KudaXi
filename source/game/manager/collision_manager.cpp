#include "collision_manager.h"

void CollisionManager::AddPrim(PrimitiveBase *_col)
{
	mPrimList.emplace_back(_col);
}

void CollisionManager::RemovePrim(PrimitiveBase *_col)
{
	//mPrimList.erase(_col);
}

void CollisionManager::UpdateCollision()
{
	// �������ō폜�m�F�H

	//for (auto itA = mColList.begin(); itA != mColList.end(); itA++)
	//{
	//	for (auto itB = itA; itB != mColList.end(); itB++)
	//	{
	//		if (itA == itB) continue;
	//		//CollisionBase* A = (*itA)->getCollisionNode();
	//		//CollisionBase* B = (*itB)->getCollisionNode();

	//		//// �����Ō^�����ʂ��Ďg�p����֐���ς�����

	//		//if (sphere2sphere((SphereNode&)*A, (SphereNode&)*B))
	//		//{
	//		//	(*itA)->OnCollision((*itB));
	//		//	(*itB)->OnCollision((*itA));
	//		//}
	//	}
	//}
	//mColList.clear();
}
