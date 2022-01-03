#pragma once
#include "gameobject.h"

namespace myUI {
	class PauseEndless : public GameObject
	{
	private:

	public:
		PauseEndless();
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectDraw()override {};
		void ObjectImguiDraw()override {};
		void Uninit() override {};
	};
}