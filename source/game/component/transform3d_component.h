#pragma once
#include "component_base.h"
#include "../../system/util/Vector.h"
#include "../../system/dx11/util/dx11mathutil.h"

class ComponentBase;

namespace Component
{
	class Transform3d : public ComponentBase
	{
	private:
		enum class MoveMode
		{
			Volume,		// 下記のMove()が呼ばれる
			Force,		// 下記のMoveByForce()が呼ばれる
		};

		MoveMode	moveMode;
		Vector3		position;
		Vector3		rotation;
		float		force;
		XMFLOAT4X4  mtx;

		// moveModeによりdirectionの扱いが異なる。
		void MoveByVolume() { position += rotation; }
		void MoveByForce() { position += rotation * force; }

		void CheckMoveMode();

	public:
		Transform3d() :position(0, 0, 0), rotation(0, 0, 0), force(0), moveMode(MoveMode::Volume) {}
		Transform3d(Vector3 pos) :position(pos), rotation(0, 0, 0), force(0), moveMode(MoveMode::Volume) {}
		Transform3d(Vector3 pos, Vector3 dir) :position(pos), rotation(dir), force(0), moveMode(MoveMode::Volume) {}
		Transform3d(Vector3 pos, Vector3 dir, float f) :position(pos), rotation(dir), force(f), moveMode(MoveMode::Volume) {}
		Transform3d(Vector3 pos, Vector3 dir, float f, MoveMode mode) :position(pos), rotation(dir), force(f), moveMode(mode) {}
		~Transform3d() {}

		Vector3		GetPosition() { return position; }
		void		SetPosition(const Vector3& newPos) { position = newPos; }

		Vector3		GetRotation() { return rotation; }
		void		SetRotation(const Vector3& newDir) { rotation = newDir; }

		float		GetForce() { return force; }
		void		SetForce(float newForce) { force = newForce; }

		MoveMode	GetMoveMode() { return moveMode; }
		void		SetMoveMode(MoveMode newMode) { moveMode = newMode; }

		XMFLOAT4X4 const GetMtx()
		{
			DX11MakeWorldMatrix(mtx, rotation, position);
			return mtx;
		}
		void SetMtx(const XMFLOAT4X4& _mtx)
		{
			mtx = _mtx;
		}

		void Update();
		//void Draw(); //このコンポーネントは描画をしないためDraw()の定義はしない。
	};
}