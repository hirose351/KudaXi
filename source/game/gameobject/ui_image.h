#pragma once
#include "gameobject.h"

namespace myUI {
	class Image : public GameObject
	{
	private:

	public:
		Image();
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectImguiDraw()override {};
		void Uninit() override {};
	};
}