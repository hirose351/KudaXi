#pragma once
#include	"component_base.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../gameobject/primitive.h"

struct CollisionData
{
	GameObject* gameObject;
	bool isHit;
	bool beforeHit;
};

namespace Component {
	class CollisionComponent : public ComponentBase
	{
	private:
		Primitive::AABB mPrim;
		std::list<Dix::sp<CollisionData>> mHitColList;

	public:
		CollisionComponent();
		~CollisionComponent();
		void Awake()override {};
		void Init()override {};
		void Update()override;
		void Draw()override;
		void Uninit()override {};

		void SetHitObj(GameObject* _hitobj);
		Primitive::AABB GetPrim() { return mPrim; }
	};
}