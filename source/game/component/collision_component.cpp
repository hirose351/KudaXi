#include "collision_component.h"
#include "../manager/collision_manager.h"

#define STR(var) #var   //引数にした変数を変数名を示す文字列リテラルとして返すマクロ関数

using namespace Component;
using namespace Dix;

CollisionComponent::CollisionComponent()
{
	CollisionManager::GetInstance().AddCollision(this);
}

CollisionComponent::~CollisionComponent()
{
	CollisionManager::GetInstance().RemoveCollision(this);
	mHitColList.clear();
}

void CollisionComponent::Init()
{
	mQube.Init(mPrim, mColor);
	DX11MakeWorldMatrix(mLocalMtx, XMFLOAT3(0, 0, 0), mLocalPos);
}

void CollisionComponent::Update()
{
	DX11MtxMultiply(mWorldMtx, mLocalMtx, mOwner->GetTransform()->GetMtx());
	mPrim.p = { mWorldMtx._41,mWorldMtx._42,mWorldMtx._43 };
	mQube.Update(mPrim);
}

void CollisionComponent::Draw()
{
	DX11MtxMultiply(mWorldMtx, mLocalMtx, mOwner->GetTransform()->GetMtx());
	mQube.Draw(mWorldMtx);
}

void CollisionComponent::ImguiDraw()
{
	if (ImGui::TreeNode("CollisionComponent"))
	{
		std::string str;

		str = "Tag";
		ImGui::Text(str.c_str());
		str = (ObjectTagStr[static_cast<int>(mTag)]);
		ImGui::Text(str.c_str());
		ImGui::Text("LocalPosition");
		ImGui::DragFloat("x", &mLocalMtx._41, 0.5f);
		ImGui::DragFloat("y", &mLocalMtx._42, 0.5f);
		ImGui::DragFloat("z", &mLocalMtx._43, 0.5f);

		for (auto& list : mHitColList)
		{
			if ((*list).isHit)
			{
				str = (*list).col->GetOwner()->GetName();
				ImGui::Text(str.c_str());
			}
		}

		ImGui::TreePop();
	}
}

void CollisionComponent::ColUpdate()
{
	// コンテナが空なら返す
	if (mHitColList.empty())
	{
		return;
	}

	/// Todo:ゲームオブジェクトがぬるの時の処理
	for (auto obj = mHitColList.begin(); obj != mHitColList.end();)
	{
		// 登録されたオブジェクトが削除されていた時の処理
		if ((*obj)->col->GetOwner()->GetObjectState() == ObjectState::eDead)
		{
			mOwner->OnCollisionExit((*obj)->col);
			obj = mHitColList.erase(obj);
			continue;
		}

		// 当たった関数呼び出し
		if ((*obj)->isHit && (*obj)->beforeHit)
		{
			mOwner->OnCollisionStay((*obj)->col);
		}
		else if (!(*obj)->isHit && (*obj)->beforeHit)
		{
			mOwner->OnCollisionExit((*obj)->col);
		}
		else if ((*obj)->isHit && !(*obj)->beforeHit)
		{
			mOwner->OnCollisionEnter((*obj)->col);
		}

		// フラグ更新
		(*obj)->beforeHit = (*obj)->isHit;
		(*obj)->isHit = false;

		obj++;
	}
	mHitColList.shrink_to_fit();
}

GameObject* CollisionComponent::GetNearestDice(Float3 _pos)
{
	CollisionComponent* colData = nullptr;
	float shortDistance = 0;
	float distance = 0;

	// リストに存在するオブジェクトがDiceであれば受け取った座標との距離を測る
	for (auto obj = mHitColList.begin(); obj != mHitColList.end(); obj++)
	{
		if ((*obj)->col->GetTag() != ObjectTag::eDice && !(*obj)->isHit && !(*obj)->beforeHit)
			continue;
		if (colData == nullptr)
		{
			colData = (*obj)->col;
			// ここが間違ってる
			DX11p2pLength(_pos, colData->GetPrim()->p, shortDistance);
			continue;
		}
		// ここが間違ってる
		DX11p2pLength(_pos, (*obj)->col->GetPrim()->p, distance);
		if (distance < shortDistance)
		{
			colData = (*obj)->col;
			shortDistance = distance;
		}
	}
	if (colData == nullptr)
		return nullptr;

	//std::cout << "保持Dice基準Y補正\n";
	//mOwner->GetTransform()->SetPositionY(colData->GetPrim()->p.y + colData->GetPrim()->hl.y + mPrim.hl.y - 0.5f);
	//mOwner->GetTransform()->CreateMtx();
	//mOwner->Update();
	return colData->GetOwner();
}

void CollisionComponent::RemoveCollisionData(CollisionComponent* _col)
{
	// リストに存在するオブジェクトか
	for (auto obj = mHitColList.begin(); obj != mHitColList.end(); obj++)
	{
		if ((*obj)->col != _col)
			continue;
		// 存在すれば消す
		mHitColList.erase(obj);
		return;
	}
}

void CollisionComponent::SetHitObj(CollisionComponent* _hitobj)
{
	// リストに存在するオブジェクトか
	for (auto obj = mHitColList.begin(); obj != mHitColList.end(); obj++)
	{
		if ((*obj)->col != _hitobj)
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
	(*colData).col = _hitobj;
	mHitColList.emplace_back(colData);
}

void CollisionComponent::SetInitState(ObjectTag _tag, Float3 _localPos, Float3 _scale, DirectX::XMFLOAT4 _color)
{
	mTag = _tag;
	mLocalPos = _localPos;
	mPrim.hl = _scale;
	mColor = _color;
}