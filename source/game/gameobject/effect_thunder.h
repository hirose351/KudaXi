#pragma once
#include "gameobject.h"

namespace Effect {
	class Thunder : public GameObject
	{
	private:
		float mAlha = 1.0f;			// α値
		Float3 mInitPos;
		const int mAlhaCnt = 40;	// α値が0になるまでに必要な更新回数
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