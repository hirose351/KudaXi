#pragma once
#include	"../../system/util/vector.h"
#include	"../manager/collision_manager.h"
#include	"gameobject.h"

#include	<math.h>

// プリミティブ定義
// v1.00
//  初出
// v1.01
// ベクトルの平行判定を外積に置き換え

#define _OX_EPSILON_	0.000001f	// 誤差

class CollisionManager;

class PrimitiveBase {
public:
	GameObject* mOwner;
	PrimitiveBase() :mOwner(nullptr) {};
	PrimitiveBase(GameObject* _owner) :mOwner(_owner) { SetManager(); };
	~PrimitiveBase() { /*削除？*/ };
	virtual PrimitiveBase* Get() { return this; }
	void SetManager();
};

namespace Primitive {

	// 点
	typedef Float3 Point;


	// ベクトル
	struct Vec3 : public Float3 {
		Vec3() {}
		Vec3(float x, float y, float z) : Float3(x, y, z) {}
		Vec3(const Float3 &r) : Float3(r) {}
		~Vec3() {}

		Vec3& operator =(const Float3 &r) {
			x = r.x;
			y = r.y;
			z = r.z;
			return *this;
		}

		// 標準化
		void norm() {
			const float len = length();
			if (len > 0.0f)
			{
				x /= len;
				y /= len;
				z /= len;
			}
		}

		// 垂直関係にある？
		bool isVertical(const Vec3 &r) const {
			float d = dot(r);
			return (-_OX_EPSILON_ < d && d < _OX_EPSILON_);	// 誤差範囲内なら垂直と判定
		}

		// 平行関係にある？
		bool isParallel(const Vec3 &r) const {
			float d = cross(r).lengthSq();
			return (-_OX_EPSILON_ < d && d < _OX_EPSILON_);	// 誤差範囲内なら平行と判定
		}

		// 鋭角関係？
		bool isSharpAngle(const Vec3 &r) const {
			return (dot(r) >= 0.0f);
		}
	};


	// 直線
	struct Line {
		Point p;
		Vec3 v;		// 方向ベクトル
		Line() : p(0.0f, 0.0f, 0.0f), v(1.0f, 0.0f, 0.0f) {}
		Line(const Point &p, const Vec3 &v) : p(p), v(v) {}
		~Line() {}

		// 点上の座標を取得
		//  ベクトルに掛け算する係数
		Point getPoint(float t) const {
			return static_cast<Float3>(p) + static_cast<Float3>(v)*t;
		}
	};


	// 線分
	struct Segment : public Primitive::Line {

		Segment() {}
		Segment(const Point &p, const Vec3 &v) : Line(p, v) {}
		Segment(const Point &p1, const Point &p2) : Line(p1, static_cast<Float3>(p2) - static_cast<Float3>(p1)) {}

		// 終点を取得
		Float3 getEndPoint() const {
			return static_cast<Float3>(p) + v;
		}
	};


	// 球
	struct Sphere :public PrimitiveBase {
		Point p;
		float r;	// 半径
		Sphere() : p(0.0f, 0.0f, 0.0f), r(0.5f) {}
		Sphere(const Point &p, float r) : p(p), r(r) {}
		~Sphere() {}
		PrimitiveBase* Get()  override {
			return this;
		}
	};

	// カプセル
	struct Capsule :public PrimitiveBase {
		Segment s;
		float r;	// 半径
		Capsule() : r(0.5f) {}
		Capsule(const Segment &s, float r) : s(s), r(r) {}
		Capsule(const Point &p1, const Point &p2, float r) : s(p1, p2), r(r) {}
		~Capsule() {}
		PrimitiveBase* Get() override {
			return this;
		}
	};

	//// AABB
	//struct AABB :public PrimitiveBase {
	//	Point p;	// 中心点
	//	Float3 hl;	// 各軸の辺の長さの半分
	//	AABB() {}
	//	AABB(const Point &p, const Float3 &hl) : p(p), hl(hl) {}

	//	// 辺の長さを取得
	//	float lenX() const { return hl.x * 2.0f; };
	//	float lenY() const { return hl.y * 2.0f; };
	//	float lenZ() const { return hl.z * 2.0f; };
	//	float len(int i) {
	//		return *((&hl.x) + i) * 2.0f;
	//	}

	//	PrimitiveBase* Get() override {
	//		return this;
	//	}
	//};

	// AABB
	struct AABB {
		Point p;	// 中心点
		Float3 hl;	// 各軸の辺の長さの半分
		AABB() {}
		AABB(const Point &p, const Float3 &hl) : p(p), hl(hl) {}

		// 辺の長さを取得
		float lenX() const { return hl.x * 2.0f; };
		float lenY() const { return hl.y * 2.0f; };
		float lenZ() const { return hl.z * 2.0f; };
		float len(int i) {
			return *((&hl.x) + i) * 2.0f;
		}
	};
}