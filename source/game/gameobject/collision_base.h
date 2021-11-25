#pragma once
#include	"transform.h"
#include	"primitive_utility.h"
#include	"gameobject.h"
#include	"../../system/util/dixsmartptr.h"

class CollisionInterface
{
	//private
	//	bool mIsHit;			// ������������
	//	bool mIsBeforeHit;		// �O����������
	//
	//protected:
	//	PrimitiveBase* mPrimitive;	// �`
	//	GameObject* mOwner;		// ������
	//
public:
	//CollisionBase() {};
	//CollisionBase(PrimitiveBase *_primitive, GameObject *_owner) :mPrimitive(_primitive), mOwner(_owner) {}
	//virtual ~CollisionBase() {};

	virtual void OnCollisionEnter() = 0;
	virtual void OnCollisionStay() = 0;
	virtual void OnCollisionExit() = 0;
	/*
		Primitive* GetPrimitive() const { return mPrimitive; };
		Dix::sp<GameObject> GetOwner() const { return mOwner; };*/
};