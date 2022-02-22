#pragma once
#include	"draw_component_base.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../gameobject/primitive.h"
#include	"qube.h"

struct CollisionData;
class DrawComponentBase;

namespace Component {
	class Collision : public DrawComponentBase
	{
	private:
		Primitive::AABB mPrim;
		std::vector<Dix::sp<CollisionData>> mpHitColList;
		Qube mQube;
		Float3 mLocalScaleHalf;
		Float3 mLocalPos;
		DirectX::XMFLOAT4 mColor{ 0, 0, 0, 0 };
		DirectX::XMFLOAT4X4 mLocalMtx;
		DirectX::XMFLOAT4X4 mWorldMtx;

	public:
		Collision();
		~Collision();

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
		void RemoveCollisionData(Collision* _col);

		void SetHitObj(Component::Collision* _hitobj);
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
	Component::Collision* col;
	bool isHit;
	bool beforeHit;
};