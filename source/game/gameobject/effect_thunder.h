#pragma once
#include "gameobject.h"

namespace Effect {
	class Thunder : public GameObject
	{
	private:
		float mAlha = 1.0f;							// —‹ƒ¿’l

	public:
		Thunder() :GameObject(("Thunder"), ObjectType::eObstracle) {};
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectDraw()override {};
		void ObjectImguiDraw()override {};
		void Uninit() override {};
	};
}