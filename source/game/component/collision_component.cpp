#include "collision_component.h"
#include "../manager/collision_manager.h"

using namespace Component;
using namespace Dix;

CollisionComponent::CollisionComponent()
{
	CollisionManager::GetInstance().AddCollision(this);
	//mQube.Init(mPrim, DirectX::XMFLOAT4(0, 1, 0, 0.5f));
	//mPrim.hl = mOwner->GetTransform()->scale / 2.0f;
}

CollisionComponent::~CollisionComponent()
{
	CollisionManager::GetInstance().RemoveCollision(this);
	mHitColList.clear();
}

void CollisionComponent::Init()
{
	//mPrim.hl = mOwner->GetTransform()->scale / 2.0f + mLocalScaleHalf;
	mPrim.p = /*mOwner->GetTransform()->GetPosition() +*/ mLocalPos;
	mQube.Init(mPrim, mColor);

	DX11MakeWorldMatrix(mLocalMtx, XMFLOAT3(0, 0, 0), mLocalPos);
}

void CollisionComponent::Update()
{
	DX11MtxMultiply(mWorldMtx, mLocalMtx, mOwner->GetTransform()->GetMtx());
	//mPrim.hl = mOwner->GetTransform()->scale / 2.0f * mLocalScaleHalf;
	mPrim.p =/* mOwner->GetTransform()->GetPosition() +*/ mLocalPos;
	mQube.Update(mPrim);
}

void CollisionComponent::Draw()
{
	mQube.Draw(mWorldMtx);
}

void CollisionComponent::ColUpdate()
{
	// コンテナが空なら返す
	if (mHitColList.empty())
	{
		return;
	}

	/// Todo:ゲームオブジェクトがぬるの時の処理
	for (auto obj = mHitColList.begin(); obj != mHitColList.end(); obj++)
	{
		// 当たった関数呼び出し
		if ((*obj)->isHit && (*obj)->beforeHit)
		{
			mOwner->OnCollisionStay((*obj)->gameObject);
		}
		else if (!(*obj)->isHit && (*obj)->beforeHit)
		{
			mOwner->OnCollisionExit((*obj)->gameObject);
		}
		else if ((*obj)->isHit && !(*obj)->beforeHit)
		{
			mOwner->OnCollisionEnter((*obj)->gameObject);
		}

		// フラグ更新
		(*obj)->beforeHit = (*obj)->isHit;
		(*obj)->isHit = false;

	}
	mHitColList.shrink_to_fit();
}

void CollisionComponent::SetHitObj(GameObject* _hitobj)
{
	// リストに存在するオブジェクトか
	for (auto obj = mHitColList.begin(); obj != mHitColList.end(); obj++)
	{
		if ((*obj)->gameObject != _hitobj)
		{
			continue;
		}
		else
		{
			(*obj)->isHit = true;
			return;
		}

	}
	// リストに新規で入れる
	sp<CollisionData> colData;
	colData.SetPtr(new CollisionData);
	(*colData).gameObject = _hitobj;
	mHitColList.emplace_back(colData);
}
