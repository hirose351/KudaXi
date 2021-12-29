#pragma once
#include "component_base.h"


namespace Component {
	namespace EasingUI {
		class EasticOut : public ComponentBase
		{
		private:
			float mTotalTime;
			float mCurrentTime;
			float mScale;
			float min;
			float max;
		public:
			void Init() override {};
			void Update()override;
			void Draw()override {};
			void ImguiDraw()override {};
			void Uninit() override {};

			void SetNum(float time, float min, float max);
			float GetNum() { return mScale; }
		};
	}
}


