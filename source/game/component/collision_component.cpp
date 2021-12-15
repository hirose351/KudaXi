#include "collision_component.h"
#include "../manager/collision_manager.h"

#define STR(var) #var   //�����ɂ����ϐ���ϐ��������������񃊃e�����Ƃ��ĕԂ��}�N���֐�

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
	// �R���e�i����Ȃ�Ԃ�
	if (mHitColList.empty())
	{
		return;
	}

	/// Todo:�Q�[���I�u�W�F�N�g���ʂ�̎��̏���
	for (auto obj = mHitColList.begin(); obj != mHitColList.end();)
	{
		// �o�^���ꂽ�I�u�W�F�N�g���폜����Ă������̏���
		if ((*obj)->col->GetOwner()->GetObjectState() == ObjectState::eDead)
		{
			mOwner->OnCollisionExit((*obj)->col);
			obj = mHitColList.erase(obj);
			continue;
		}

		// ���������֐��Ăяo��
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

		// �t���O�X�V
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

	// ���X�g�ɑ��݂���I�u�W�F�N�g��Dice�ł���Ύ󂯎�������W�Ƃ̋����𑪂�
	for (auto obj = mHitColList.begin(); obj != mHitColList.end(); obj++)
	{
		if ((*obj)->col->GetTag() != ObjectTag::eDice && !(*obj)->isHit && !(*obj)->beforeHit)
			continue;
		if (colData == nullptr)
		{
			colData = (*obj)->col;
			// �������Ԉ���Ă�
			DX11p2pLength(_pos, colData->GetPrim()->p, shortDistance);
			continue;
		}
		// �������Ԉ���Ă�
		DX11p2pLength(_pos, (*obj)->col->GetPrim()->p, distance);
		if (distance < shortDistance)
		{
			colData = (*obj)->col;
			shortDistance = distance;
		}
	}
	if (colData == nullptr)
		return nullptr;

	//std::cout << "�ێ�Dice�Y�␳\n";
	//mOwner->GetTransform()->SetPositionY(colData->GetPrim()->p.y + colData->GetPrim()->hl.y + mPrim.hl.y - 0.5f);
	//mOwner->GetTransform()->CreateMtx();
	//mOwner->Update();
	return colData->GetOwner();
}

void CollisionComponent::RemoveCollisionData(CollisionComponent* _col)
{
	// ���X�g�ɑ��݂���I�u�W�F�N�g��
	for (auto obj = mHitColList.begin(); obj != mHitColList.end(); obj++)
	{
		if ((*obj)->col != _col)
			continue;
		// ���݂���Ώ���
		mHitColList.erase(obj);
		return;
	}
}

void CollisionComponent::SetHitObj(CollisionComponent* _hitobj)
{
	// ���X�g�ɑ��݂���I�u�W�F�N�g��
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
	// ���X�g�ɐV�K�œ����
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