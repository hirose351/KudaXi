#pragma once
#include	"component_base.h"
#include	"../../system/util/easing.h"
#include	<list>

// 動かすもの
enum class TransType
{
	ePos,
	eRot,
	eScale
};

namespace Component {
	class EasingImage : public ComponentBase
	{
	private:
		// イージングに必要な固まり
		struct EasingFamily
		{
			Easing::EasingType easingType = Easing::EasingType::eLinear;
			TransType transType = TransType::ePos;
			float totalTime = 0.0f;							// 必要な時間
			DirectX::XMFLOAT2 startValue = { 0.0f,0.0f };	// はじめの数
			DirectX::XMFLOAT2 endValue = { 0.0f,0.0f };		// 終わりの数
		};

		float mTotalTime;	 // 
		float mCurrentTime;  // 経過時間
		float mEndValue;	 // 

		bool isStart = false;

		std::list<EasingFamily> mEasingList;
		XMFLOAT2 mStartValue = { 0, 0 };

	public:
		void Init() override {};
		void Update()override;
		void Draw()override {};
		void ImguiDraw()override {};
		void Uninit() override {};

		// startValueが0,0なら相対座標
		void AddEasing(Easing::EasingType _easingType,
					   TransType _transType,
					   float _totalTime,
					   DirectX::XMFLOAT2 _startValue,
					   DirectX::XMFLOAT2 _endValue);
	};
}


