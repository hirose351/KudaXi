#pragma once
#include "gameobject.h"

namespace UI {
	class Button : public GameObject
	{
	private:
		bool m;


	public:
		Button();
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectDraw()override {};
		void ObjectImguiDraw()override {};
		void Uninit() override {};
	};
}