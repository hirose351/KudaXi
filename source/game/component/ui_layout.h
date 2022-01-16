#pragma once
#include	"component_base.h"

// �J�n�n�_
enum class StartPoint
{
	eLeftUp,
	eLeftDown,
	eRightUp,
	eRightDown
};

namespace Component {
	class Layout : public ComponentBase
	{
	private:
		DirectX::XMFLOAT2 mSpace;
		StartPoint mStartP;
		unsigned int mHeightCnt;

	public:
		Layout();
		void Init() override {};
		void Update()override;
		void ImguiDraw()override {};
		void Uninit() override {};
	};
}

