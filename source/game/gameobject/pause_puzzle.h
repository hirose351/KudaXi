#pragma once
#include "gameobject.h"

namespace myUI {
	class PausePuzzle : public GameObject
	{
	private:

	public:
		PausePuzzle();
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectImguiDraw()override {};
		void Uninit() override {};
	};
}