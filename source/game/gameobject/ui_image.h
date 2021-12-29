#pragma once
#include "gameobject.h"
#include "../component/easing_easticout_component.h"

namespace myUI {
	class Image : public GameObject
	{
	private:



	public:
		Image() :GameObject(("Image"), ObjectType::eObstracle) {
			AddComponent<Component::EasingUI::EasticOut>()->SetNum(100.0f, 0.0f, 500.0f);
		};
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectDraw()override {};
		void ObjectImguiDraw()override {};
		void Uninit() override {};
	};
}