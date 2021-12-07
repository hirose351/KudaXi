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

float hosei(float aHl, float aP, float bHl, float bP)
{
	if (aP < bP)
		return -((aHl + bHl) - std::abs(aP - bP));
	return (aHl + bHl) - std::abs(aP - bP);
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

			Float3 ans(((a.hl.x + b.hl.x) - std::abs(a.p.x - b.p.x)),
				(a.hl.y + b.hl.y) - std::abs(a.p.y - b.p.y),
					   (a.hl.z + b.hl.z) - std::abs(a.p.z - b.p.z));

			if (ans.x < 0)continue;
			if (ans.y < 0)continue;
			if (ans.z < 0)continue;

			(*itA)->SetHitObj((*itB).GetPtr());
			(*itB)->SetHitObj((*itA).GetPtr());

			/*if ((*itA).GetPtr()->GetOwner()->GetObjectType() == ObjectType::ePlayer)
			{
				if (ans.x < ans.y&&ans.x < ans.z)
				{
					std::cout << "X•â³\n";
					(*itA).GetPtr()->GetOwner()->GetTransform()->PositionCorrectionX(hosei(a.hl.x, a.p.x, b.hl.x, b.p.x));
				}
				else if (ans.y < ans.z)
				{
					std::cout << "Y•â³\n";
					(*itA).GetPtr()->GetOwner()->GetTransform()->SetPositionY(b.p.y + b.hl.y + a.hl.y - 0.5f);
					(*itA).GetPtr()->GetOwner()->GetTransform()->CreateMtx();
					(*itA).GetPtr()->Update();
				}
				else
				{
					std::cout << "Z•â³\n";
					(*itA).GetPtr()->GetOwner()->GetTransform()->PositionCorrectionZ(hosei(a.hl.z, a.p.z, b.hl.z, b.p.z));
				}
				(*itA).GetPtr()->GetOwner()->GetTransform()->CreateMtx();
			}
			else if ((*itB).GetPtr()->GetOwner()->GetObjectType() == ObjectType::ePlayer)
			{
				if (ans.x < ans.y&&ans.x < ans.z)
				{
					std::cout << "X•â³\n";
					(*itB).GetPtr()->GetOwner()->GetTransform()->PositionCorrectionX(hosei(a.hl.x, a.p.x, b.hl.x, b.p.x));
				}
				else if (ans.y < ans.z)
				{
					std::cout << "Y•â³\n";
					(*itB).GetPtr()->GetOwner()->GetTransform()->PositionCorrectionY(hosei(a.hl.y, a.p.y, b.hl.y, b.p.y));
				}
				else
				{
					std::cout << "Z•â³\n";
					(*itB).GetPtr()->GetOwner()->GetTransform()->PositionCorrectionZ(hosei(a.hl.z, a.p.z, b.hl.z, b.p.z));
				}
				(*itB).GetPtr()->GetOwner()->GetTransform()->CreateMtx();
			}*/
		}
	}

	for (auto it = mColList.begin(); it != mColList.end(); it++)
	{
		(*it)->ColUpdate();
	}
}
