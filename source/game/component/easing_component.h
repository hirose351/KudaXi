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
	class Easing : public ComponentBase
	{
	private:
		// イージングに必要な固まり
		struct EasingFamily
		{
			EasingProcess::EasingType easingType = EasingProcess::EasingType::eLinear;
			TransType transType = TransType::ePos;
			float totalFrame = 0.0f;						// 必要な時間
			float delayFrame = 0.0f;						// 遅延時間
			Float3 startValue;								// 開始時の値
			Float3 endValue;								// 終了時の値
			bool isStartAbsolute;							// 開始時の値が相対値か
			bool isEndAbsolute;								// 終了時の値が相対値か
		};

		float mCurrentFrame;								// 経過時間
		bool isStart = false;								// 開始したか
		std::list<EasingFamily> mEasingList;				// イージングの塊が入ったリスト
		bool mIsRepeat = false;								// 繰り返すか

	public:
		Easing();
		void Init() override {};
		void Update()override;
		void ImguiDraw()override {};
		void Uninit() override {};

		// イージング追加
		void AddEasing(EasingProcess::EasingType _easingType,
					   TransType _transType,
					   float _totalFrame,
					   float _delayFrame,
					   Float3 _startValue,
					   Float3 _endValue,
					   bool _isStartAbsolute = false,
					   bool _isEndAbsolute = false);

		// リストに存在するイージング処理の数を返す
		int GetEasingListCnt();

		void SetIsRepeat(bool _flg) { mIsRepeat = _flg; }
	};
}


