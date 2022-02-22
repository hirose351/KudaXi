#pragma once
#include	"component_base.h"

namespace Component {

	class MapPos;
	class MapMove : public ComponentBase
	{
	private:
		Component::MapPos* mpMapPos;
		ObjectType mOwnerType;

	public:
		MapMove();

		void Awake()override {};
		void Init()override;
		void Update()override;
		void ImguiDraw()override;
		void Uninit()override {};
	};
}
