#pragma once
#include	"component_base.h"

namespace Component {
	class Animation2d : public ComponentBase
	{
	private:

	public:
		Animation2d();
		void Init() override {};
		void Update()override {};
		void ImguiDraw()override {};
		void Uninit() override {};

		// アニメーション設定
		// 速度、縦何列目か
		void SetAnim(float _speed, int array);
	};
}