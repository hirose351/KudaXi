#pragma once
#include	"component_base.h"

namespace Component {
	class Quad2d;
	class Number : public ComponentBase
	{
	private:
		int mNum = 0;				// •\¦‚·‚é”
		float mSpace = 0;			// ŠÔŠu
		float mStartingPoint = 0;	// n“_
		Quad2d* mQuad;
		int mDigit = 0;				// Œ…”
		bool mIsDigitFixed = false;	// Œ…”ŒÅ’è‚©

	public:
		Number();
		void Init() override;
		void Update()override;
		void ImguiDraw()override;
		void Uninit() override {};

		void SetNum(int _num, float _space = 0.0f, int _digit = 0);
	};
}