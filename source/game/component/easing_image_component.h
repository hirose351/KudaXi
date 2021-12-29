#pragma once
#include "component_base.h"


namespace Component {
	class EsingImage : public ComponentBase
	{
	private:
		float mTotalTime;
		float mCurrentTime;
		float mScale;
		float mMin;
		float mMax;

	public:
		void Init() override {};
		void Update()override;
		void Draw()override {};
		void ImguiDraw()override {};
		void Uninit() override {};

		void SetNum(float _time, float _min, float _max);
		float GetNum() { return mScale; }
	};
}


