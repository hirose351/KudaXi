#include "collision_manager.h"

//using namespace Component;

void CollisionManager::AddCollision(Component::CollisionComponent *_col)
{
	mColList.emplace_back(_col);
}

void CollisionManager::RemoveCollision(Component::CollisionComponent *_col)
{
	//mColList.erase(_col);
}

float hisei(float aHl, float aP, float bHl, float bP)
{
	if (aP < bP)
	{
		if (-((aHl + bHl) - std::abs(aP - bP)) < -0.5f)
		{
			return -((aHl + bHl) - std::abs(aP - bP));
		}
		return -((aHl + bHl) - std::abs(aP - bP));
	}
	else
	{
		if (((aHl + bHl) - std::abs(aP - bP)) > 0.5f)
		{
			return (aHl + bHl) - std::abs(aP - bP);
		}
		return (aHl + bHl) - std::abs(aP - bP);
	}
}
void CollisionManager::Update()
{
	for (auto itA = mColList.begin(); itA != mColList.end(); itA++)
	{
		for (auto itB = itA; itB != mColList.end(); itB++)
		{
			if (itA == itB) continue;
			Primitive::AABB a = *(*itA)->GetPrim();
			Primitive::AABB b = *(*itB)->GetPrim();

			if ((a.hl.x + b.hl.x) < std::abs(a.p.x - b.p.x)) continue;
			if ((a.hl.y + b.hl.y) < std::abs(a.p.y - b.p.y)) continue;
			if ((a.hl.z + b.hl.z) < std::abs(a.p.z - b.p.z)) continue;

			(*itA)->SetHitObj((*itB).GetPtr());
			(*itB)->SetHitObj((*itA).GetPtr());

			Float3 ans(((a.hl.x + b.hl.x) - std::abs(a.p.x - b.p.x)),
				(a.hl.y + b.hl.y) - std::abs(a.p.y - b.p.y),
					   (a.hl.z + b.hl.z) - std::abs(a.p.z - b.p.z));

			if ((*itA).GetPtr()->GetOwner()->GetObjectType() == ObjectType::Player)
			{
				if (ans.x < ans.y&&ans.x < ans.z)
				{
					std::cout << "X•â³\n";
					(*itA).GetPtr()->GetOwner()->GetTransform()->PositionCorrectionX(hisei(a.hl.x, a.p.x, b.hl.x, b.p.x));
				}
				else if (ans.y < ans.z)
				{
					std::cout << "Y•â³\n";
					(*itA).GetPtr()->GetOwner()->GetTransform()->PositionCorrectionY(hisei(a.hl.y, a.p.y, b.hl.y, b.p.y));
				}
				else
				{
					std::cout << "Z•â³\n";
					(*itA).GetPtr()->GetOwner()->GetTransform()->PositionCorrectionZ(hisei(a.hl.z, a.p.z, b.hl.z, b.p.z));
				}
				(*itA).GetPtr()->GetOwner()->GetTransform()->CreateMtx();
			}
			else if ((*itB).GetPtr()->GetOwner()->GetObjectType() == ObjectType::Player)
			{
				if (ans.x < ans.y&&ans.x < ans.z)
				{
					std::cout << "X•â³\n";
					(*itB).GetPtr()->GetOwner()->GetTransform()->PositionCorrectionX(hisei(a.hl.x, a.p.x, b.hl.x, b.p.x));
				}
				else if (ans.y < ans.z)
				{
					std::cout << "Y•â³\n";
					(*itB).GetPtr()->GetOwner()->GetTransform()->PositionCorrectionY(hisei(a.hl.y, a.p.y, b.hl.y, b.p.y));
				}
				else
				{
					std::cout << "Z•â³\n";
					(*itB).GetPtr()->GetOwner()->GetTransform()->PositionCorrectionY(hisei(a.hl.y, a.p.y, b.hl.y, b.p.y));
				}
				(*itB).GetPtr()->GetOwner()->GetTransform()->CreateMtx();
			}
		}

	}

	for (auto it = mColList.begin(); it != mColList.end(); it++)
	{
		(*it)->ColUpdate();
	}
}
