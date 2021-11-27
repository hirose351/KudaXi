#pragma once
#include	"../../system/util/vector.h"
#include	"../manager/collision_manager.h"
#include	"gameobject.h"

#include	<math.h>

// �v���~�e�B�u��`
// v1.00
//  ���o
// v1.01
// �x�N�g���̕��s������O�ςɒu������

#define _OX_EPSILON_	0.000001f	// �덷

class CollisionManager;

class PrimitiveBase {
public:
	GameObject* mOwner;
	PrimitiveBase() :mOwner(nullptr) {};
	PrimitiveBase(GameObject* _owner) :mOwner(_owner) { SetManager(); };
	~PrimitiveBase() { /*�폜�H*/ };
	virtual PrimitiveBase* Get() { return this; }
	void SetManager();
};

namespace Primitive {

	// �_
	typedef Float3 Point;


	// �x�N�g��
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

		// �W����
		void norm() {
			const float len = length();
			if (len > 0.0f)
			{
				x /= len;
				y /= len;
				z /= len;
			}
		}

		// �����֌W�ɂ���H
		bool isVertical(const Vec3 &r) const {
			float d = dot(r);
			return (-_OX_EPSILON_ < d && d < _OX_EPSILON_);	// �덷�͈͓��Ȃ琂���Ɣ���
		}

		// ���s�֌W�ɂ���H
		bool isParallel(const Vec3 &r) const {
			float d = cross(r).lengthSq();
			return (-_OX_EPSILON_ < d && d < _OX_EPSILON_);	// �덷�͈͓��Ȃ畽�s�Ɣ���
		}

		// �s�p�֌W�H
		bool isSharpAngle(const Vec3 &r) const {
			return (dot(r) >= 0.0f);
		}
	};


	// ����
	struct Line {
		Point p;
		Vec3 v;		// �����x�N�g��
		Line() : p(0.0f, 0.0f, 0.0f), v(1.0f, 0.0f, 0.0f) {}
		Line(const Point &p, const Vec3 &v) : p(p), v(v) {}
		~Line() {}

		// �_��̍��W���擾
		//  �x�N�g���Ɋ|���Z����W��
		Point getPoint(float t) const {
			return static_cast<Float3>(p) + static_cast<Float3>(v)*t;
		}
	};


	// ����
	struct Segment : public Primitive::Line {

		Segment() {}
		Segment(const Point &p, const Vec3 &v) : Line(p, v) {}
		Segment(const Point &p1, const Point &p2) : Line(p1, static_cast<Float3>(p2) - static_cast<Float3>(p1)) {}

		// �I�_���擾
		Float3 getEndPoint() const {
			return static_cast<Float3>(p) + v;
		}
	};


	// ��
	struct Sphere :public PrimitiveBase {
		Point p;
		float r;	// ���a
		Sphere() : p(0.0f, 0.0f, 0.0f), r(0.5f) {}
		Sphere(const Point &p, float r) : p(p), r(r) {}
		~Sphere() {}
		PrimitiveBase* Get()  override {
			return this;
		}
	};

	// �J�v�Z��
	struct Capsule :public PrimitiveBase {
		Segment s;
		float r;	// ���a
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
	//	Point p;	// ���S�_
	//	Float3 hl;	// �e���̕ӂ̒����̔���
	//	AABB() {}
	//	AABB(const Point &p, const Float3 &hl) : p(p), hl(hl) {}

	//	// �ӂ̒������擾
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
		Point p;	// ���S�_
		Float3 hl;	// �e���̕ӂ̒����̔���
		AABB() {}
		AABB(const Point &p, const Float3 &hl) : p(p), hl(hl) {}

		// �ӂ̒������擾
		float lenX() const { return hl.x * 2.0f; };
		float lenY() const { return hl.y * 2.0f; };
		float lenZ() const { return hl.z * 2.0f; };
		float len(int i) {
			return *((&hl.x) + i) * 2.0f;
		}
	};
}