#pragma once
#include	"component_base.h"

namespace Component {
	class Quad2d;
	class Number : public ComponentBase
	{
	private:
		int mNum = 0;				// 表示する数
		float mSpace = 0;			// 間隔
		float mStartingPoint = 0;	// 始点
		Quad2d* mQuad;
		int mDigit = 0;				// 桁数
		bool mIsDigitFixed = false;	// 桁数固定か

	public:
		Number();
		void Init() override;
		void Update()override;
		void ImguiDraw()override;
		void Uninit() override {};

		void SetNum(int _num, float _space = 0.0f, int _digit = 0);
	};
}