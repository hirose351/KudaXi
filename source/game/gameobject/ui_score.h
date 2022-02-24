#pragma once
#include "gameobject.h"

namespace myUI {
	class Score : public GameObject
	{
	private:

	public:
		Score();
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectImguiDraw()override;
		void Uninit() override {};
	};
}