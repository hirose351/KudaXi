#pragma once
#include "component_base.h"

namespace Component {
	class MapMove : public ComponentBase
	{
	public:
		MapMove();

		void Awake()override {};
		void Init()override;
		void Update()override;
		void ImguiDraw()override;
		void Uninit()override {};
	};
}
