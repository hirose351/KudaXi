#pragma once
#include "gameobject.h"

namespace Effect {
	class Thunder : public GameObject
	{
	private:
		Float3 mInitPos;
	public:
		Thunder() :GameObject(("Thunder"), ObjectType::eObstracle, false) {};
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectImguiDraw()override {};
		void Uninit() override {};

		void SetInitPos(const Float3& _pos) { mInitPos = _pos; };
	};
}