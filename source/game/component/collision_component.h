#pragma once
#include	"component_base.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../gameobject/primitive.h"
#include	"qube.h"

struct CollisionData;
class ComponentBase;

namespace Component {
	class CollisionComponent : public ComponentBase
	{
	private:
		Primitive::AABB mPrim;
		std::vector<Dix::sp<CollisionData>> mHitColList;
		Qube mQube;
		Float3 mLocalScaleHalf;
		Float3 mLocalPos;
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
		void ImguiDraw()override;
		void Uninit()override {};

		void ColUpdate();

		// 受け取った座標から最も近いDiceを返す
		GameObject* GetNearestDice(Float3 _pos);

		// 指定されたコンポーネントが入ったデータを削除
		void RemoveCollisionData(CollisionComponent* _col);

		void SetHitObj(Component::CollisionComponent* _hitobj);
		void SetLocalScale(Float3 _scale) { mPrim.hl = _scale; };
		void SetLocalPos(Float3 _localPos) { mLocalPos = _localPos; };
		void SetColor(DirectX::XMFLOAT4 _color) { mColor = _color; mQube.SetColor(mColor); };

		void SetInitState(ObjectTag _tag, Float3 _localPos, Float3 _scale, DirectX::XMFLOAT4 _color);


		Primitive::AABB* GetPrim() { Update(); return &mPrim; }
	};
}

struct CollisionData
{
	CollisionData() :isHit(false), beforeHit(false) {}
	virtual ~CollisionData() {};
	Component::CollisionComponent* col;
	bool isHit;
	bool beforeHit;
};