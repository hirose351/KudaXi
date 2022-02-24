#pragma once
#include "gameobject.h"

namespace Effect {
	class Thunder : public GameObject
	{
	private:
		float mAlha = 1.0f;			// ���l
		Float3 mInitPos;
		const int mAlhaCnt = 40;	// ���l��0�ɂȂ�܂łɕK�v�ȍX�V��
		const float mAlhaPerFrame = 1.0f / mAlhaCnt;
	public:
		Thunder() :GameObject(("Thunder"), ObjectType::eObstracle, false) {};
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectImguiDraw()override {};
		void Uninit() override {};

		void SetInitPos(const Float3& _pos) { mInitPos = _pos; };
	};
}