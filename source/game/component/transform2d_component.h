#pragma once
#include "component_base.h"
#include "../../system/util/Vector.h"

//class ComponentBase;

namespace Component
{
	class Transform2d : public ComponentBase
	{
	private:
		enum class MoveMode
		{
			Volume,//���L��Move()���Ă΂��
			Force,//���L��MoveByForce()���Ă΂��
		};

		MoveMode	moveMode;
		Vector2		position;
		Vector2		direction;
		float		force;

		//moveMode�ɂ��direction�̈������قȂ�B
		void MoveByVolume() { position += direction; }
		void MoveByForce() { position += direction * force; }

		void CheckMoveMode();

	public:
		Transform2d() :position(0, 0), direction(0, 0), force(0), moveMode(MoveMode::Volume) {}
		Transform2d(Vector2 pos) :position(pos), direction(0, 0), force(0), moveMode(MoveMode::Volume) {}
		Transform2d(Vector2 pos, Vector2 dir) :position(pos), direction(dir), force(0), moveMode(MoveMode::Volume) {}
		Transform2d(Vector2 pos, Vector2 dir, float f) :position(pos), direction(dir), force(f), moveMode(MoveMode::Volume) {}
		Transform2d(Vector2 pos, Vector2 dir, float f, MoveMode mode) :position(pos), direction(dir), force(f), moveMode(mode) {}
		~Transform2d() {}

		Vector2		GetPosition() { return position; }
		void		SetPosition(Vector2 newPos) { position = newPos; }

		Vector2		GetDirection() { return direction; }
		void		SetDirection(Vector2 newDir) { direction = newDir; }

		float		GetForce() { return force; }
		void		SetForce(float newForce) { force = newForce; }

		MoveMode	GetMoveMode() { return moveMode; }
		void		SetMoveMode(MoveMode newMode) { moveMode = newMode; }

		void Update();
		//void Draw(); //���̃R���|�[�l���g�͕`������Ȃ�����Draw()�̒�`�͂��Ȃ��B
	};
}