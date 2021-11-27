#pragma once
#include	"transform.h"
#include	"primitive.h"
#include	"gameobject.h"
#include	"../../system/util/dixsmartptr.h"

class CollisionInterface
{
	//private
	//	bool mIsHit;			// ç°ìñÇΩÇ¡ÇΩÇ©
	//	bool mIsBeforeHit;		// ëOìñÇΩÇ¡ÇΩÇ©
	//
	//protected:
	//	PrimitiveBase* mPrimitive;	// å`
	//	GameObject* mOwner;		// éùÇøéÂ
	//
public:
	//CollisionBase() {};
	//CollisionBase(PrimitiveBase *_primitive, GameObject *_owner) :mPrimitive(_primitive), mOwner(_owner) {}
	//virtual ~CollisionBase() {};

	virtual void OnCollisionEnter(GameObject* _oher) = 0;
	virtual void OnCollisionStay(GameObject* _oher) = 0;
	virtual void OnCollisionExit(GameObject* _oher) = 0;
	/*
		Primitive* GetPrimitive() const { return mPrimitive; };
		Dix::sp<GameObject> GetOwner() const { return mOwner; };*/
};