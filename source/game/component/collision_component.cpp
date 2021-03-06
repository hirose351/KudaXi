#include "collision_component.h"
#include "../manager/collision_manager.h"

#define STR(var) #var   //引数にした変数を変数名を示す文字列リテラルとして返すマクロ関数

using namespace Component;
using namespace Dix;

Collision::Collision()
{
	SetName("Collision");
	CollisionManager::GetInstance().AddCollision(this);
}

Collision::~Collision()
{
	CollisionManager::GetInstance().RemoveCollision(this);
	mpHitColList.clear();
}

void Collision::Init()
{
	mQube.Init(mPrim, mColor);
}

void Collision::Update()
{
	mPrim.p = { mWorldMtx._41,mWorldMtx._42,mWorldMtx._43 };
	mQube.Update(mPrim);
}

void Collision::Draw()
{
	mQube.Draw(mWorldMtx);
}

void Collision::ImguiDrawComponent()
{
	std::string str;

	str = "Tag";
	ImGui::Text(str.c_str());
	str = (ObjectTagStr[static_cast<int>(mTag)]);
	ImGui::Text(str.c_str());

	for (auto& list : mpHitColList)
	{
		if ((*list).isHit)
		{
			str = (*list).col->GetOwner()->GetName();
			ImGui::Text(str.c_str());
		}
	}
}

void Collision::ColUpdate()
{
	// コンテナが空なら返す
	if (mpHitColList.empty())
	{
		return;
	}

	/// Todo:ゲームオブジェクトがぬるの時の処理
	for (auto obj = mpHitColList.begin(); obj != mpHitColList.end();)
	{
		// 登録されたオブジェクトが削除されていた時の処理
		if ((*obj)->col->GetOwner()->GetObjectState() == ObjectState::eDead)
		{
			mpOwner->OnCollisionExit((*obj)->col);
			obj = mpHitColList.erase(obj);
			continue;
		}

		// 当たった関数呼び出し
		if ((*obj)->isHit && (*obj)->beforeHit)
		{
			mpOwner->OnCollisionStay((*obj)->col);
		}
		else if (!(*obj)->isHit && (*obj)->beforeHit)
		{
			mpOwner->OnCollisionExit((*obj)->col);
		}
		else if ((*obj)->isHit && !(*obj)->beforeHit)
		{
			mpOwner->OnCollisionEnter((*obj)->col);
		}

		// フラグ更新
		(*obj)->beforeHit = (*obj)->isHit;
		(*obj)->isHit = false;

		obj++;
	}
	mpHitColList.shrink_to_fit();
}

GameObject* Collision::GetNearestDice(Float3 _pos)
{
	Collision* colData = nullptr;
	float shortDistance = 0;
	float distance = 0;

	// リストに存在するオブジェクトがDiceであれば受け取った座標との距離を測る
	for (auto obj = mpHitColList.begin(); obj != mpHitColList.end(); obj++)
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

void Collision::RemoveCollisionData(Collision* _col)
{
	// リストに存在するオブジェクトか
	for (auto obj = mpHitColList.begin(); obj != mpHitColList.end(); obj++)
	{
		if ((*obj)->col != _col)
			continue;
		// 存在すれば消す
		mpHitColList.erase(obj);
		return;
	}
}

void Collision::SetHitObj(Collision* _hitobj)
{
	// リストに存在するオブジェクトか
	for (auto obj = mpHitColList.begin(); obj != mpHitColList.end(); obj++)
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
	mpHitColList.emplace_back(colData);
}

void Collision::SetInitState(ObjectTag _tag, Float3 _localPos, Float3 _scale, DirectX::XMFLOAT4 _color)
{
	mTag = _tag;
	mLocalPos = _localPos;
	mPrim.hl = _scale;
	mColor = _color;
}