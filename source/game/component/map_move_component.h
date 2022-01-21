#pragma once
#include	"component_base.h"

///* �I������Ă�Ƃ��ɃA�N�e�B�u�ɂ���
///* 
///* 
///* 

namespace Component {

	class MapPos;
	class MapMove : public ComponentBase
	{
	private:
		Component::MapPos* pos;
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
