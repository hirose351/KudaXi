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
	mQube.Draw(mWorldMtx);
}

void CollisionComponent::ColUpdate()
{
	// �R���e�i����Ȃ�Ԃ�
	if (mHitColList.empty())
	{
		return;
	}

	/// Todo:�Q�[���I�u�W�F�N�g���ʂ�̎��̏���
	for (auto obj = mHitColList.begin(); obj != mHitColList.end(); obj++)
	{
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

	}
	mHitColList.shrink_to_fit();
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

void Component::CollisionComponent::SetInitState(ObjectTag _tag, Float3 _localPos, Float3 _scale, DirectX::XMFLOAT4 _color)
{
	mTag = _tag;
	mLocalPos = _localPos;
	mPrim.hl = _scale;
	mColor = _color;
}