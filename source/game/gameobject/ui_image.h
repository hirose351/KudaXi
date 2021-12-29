#pragma once
#include "gameobject.h"

namespace myUI {
	class Image : public GameObject
	{
	private:

	public:
		Image() :GameObject(("Image"), ObjectType::eObstracle) {};
		void ObjectInit() override;
		void ObjectUpdate()override {};
		void ObjectDraw()override {};
		void ObjectImguiDraw()override {};
		void Uninit() override {};
	};
}