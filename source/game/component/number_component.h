#pragma once
#include	"component_base.h"

namespace Component {
	class Number : public ComponentBase
	{
	private:
		int mNum = 0;				// ï\é¶Ç∑ÇÈêî
		float mSpace = 0;			// ä‘äu
		float mStartingPoint = 0;	// énì_

	public:
		Number();
		void Init() override {};
		void Update()override;
		void ImguiDraw()override;
		void Uninit() override {};

		void SetNum(int _num, float _space = 0.0f);
	};
}