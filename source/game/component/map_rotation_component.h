#pragma once
#include "component_base.h"

namespace Component {
	class MapRotation : public ComponentBase
	{
	public:
		MapRotation();

		void Awake()override {};
		void Init()override;
		void Update()override;
		void ImguiDraw()override;
		void Uninit()override {};
	};
}