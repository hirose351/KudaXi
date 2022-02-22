#include "collision_manager.h"

CollisionManager::~CollisionManager()
{
	mpColList.clear();
	mpColList.shrink_to_fit();
}

void CollisionManager::AddCollision(Component::Collision *_col)
{
	//mColList.emplace_back(_col);
}

void CollisionManager::RemoveCollision(Component::Collision *_col)
{
	//auto it = std::find(mColList.begin(), mColList.end(), _col);
	//mColList.erase(it);
}

float hosei(float aHl, float aP, float bHl, float bP)
{
	if (aP < bP)
		return -((aHl + bHl) - std::abs(aP - bP));
	return (aHl + bHl) - std::abs(aP - bP);
}
void CollisionManager::Update()
{
	//for (auto itA = mColList.begin(); itA != mColList.end(); itA++)
	//{
	//	for (auto itB = itA; itB != mColList.end(); itB++)
	//	{
	//		if (itA == itB) continue;
	//		Primitive::AABB a = *(*itA)->GetPrim();
	//		Primitive::AABB b = *(*itB)->GetPrim();

	//		Float3 ans(((a.hl.x + b.hl.x) - std::abs(a.p.x - b.p.x)),
	//			(a.hl.y + b.hl.y) - std::abs(a.p.y - b.p.y),
	//				   (a.hl.z + b.hl.z) - std::abs(a.p.z - b.p.z));

	//		if (ans.x < 0)continue;
	//		if (ans.y < 0)continue;
	//		if (ans.z < 0)continue;

	//		(*itA)->SetHitObj(*itB);
	//		(*itB)->SetHitObj(*itA);

	//		if ((*itA)->GetOwner()->GetObjectType() == ObjectType::ePlayer && (*itA)->GetOwner()->GetFoot() == Foot::eFloor)
	//		{
	//			//if ((*itB)->GetPrim()->p.y <= DICE_SCALE_HALF)
	//			//	continue;

	//			if (ans.x < ans.z)
	//			{
	//				std::cout << "X•â³\n";
	//				(*itA)->GetOwner()->GetTransform()->PositionCorrectionX(hosei(a.hl.x, a.p.x, b.hl.x, b.p.x));
	//			}
	//			else
	//			{
	//				std::cout << "Z•â³\n";
	//				(*itA)->GetOwner()->GetTransform()->PositionCorrectionZ(hosei(a.hl.z, a.p.z, b.hl.z, b.p.z));
	//			}
	//			(*itA)->GetOwner()->GetTransform()->CreateMtx();
	//		}
	//		else if ((*itB)->GetOwner()->GetObjectType() == ObjectType::ePlayer && (*itA)->GetOwner()->GetFoot() == Foot::eFloor)
	//		{
	//			//if ((*itA)->GetPrim()->p.y <= DICE_SCALE_HALF)
	//			//	continue;

	//			if (ans.x < ans.z)
	//			{
	//				std::cout << "X•â³\n";
	//				(*itB)->GetOwner()->GetTransform()->PositionCorrectionX(hosei(a.hl.x, a.p.x, b.hl.x, b.p.x));
	//			}
	//			else
	//			{
	//				std::cout << "Z•â³\n";
	//				(*itB)->GetOwner()->GetTransform()->PositionCorrectionZ(hosei(a.hl.z, a.p.z, b.hl.z, b.p.z));
	//			}
	//			(*itB)->GetOwner()->GetTransform()->CreateMtx();
	//		}
	//	}
	//}

	//for (auto it = mColList.begin(); it != mColList.end(); it++)
	//{
	//	(*it)->ColUpdate();
	//}
}
