#pragma once
#include	"component_base.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../gameobject/primitive.h"
#include	"Qube.h"

struct CollisionData
{
	CollisionData() :isHit(false), beforeHit(false) {}
	GameObject* gameObject;
	bool isHit;
	bool beforeHit;
};

namespace Component {
	class CollisionComponent : public ComponentBase
	{
	private:
		Primitive::AABB mPrim;
		std::vector<Dix::sp<CollisionData>> mHitColList;
		Qube mQube;
		Float3 mLocalScaleHalf;
		Float3 mLocalPos;
		ColliderTag mTag;
		DirectX::XMFLOAT4 mColor{ 1, 1, 1, 1 };
		DirectX::XMFLOAT4X4 mLocalMtx;
		DirectX::XMFLOAT4X4 mWorldMtx;

	public:
		CollisionComponent();
		~CollisionComponent();
		void Awake()override {};
		void Init()override;
		void Update()override;
		void Draw()override;
		void Uninit()override {};

		void ColUpdate();

		void SetHitObj(GameObject* _hitobj);
		void SetLocalScale(Float3 _scale) { mPrim.hl = _scale; };
		void SetLocalPos(Float3 _localPos) { mLocalPos = _localPos; };
		void SetColor(DirectX::XMFLOAT4 _color) { mColor = _color; };

		void SetInitState(ColliderTag _tag, Float3 _localPos, Float3 _scale, DirectX::XMFLOAT4 _color);

		ColliderTag GetTag() { return mTag; }
		Primitive::AABB* GetPrim() { return &mPrim; }
	};
}