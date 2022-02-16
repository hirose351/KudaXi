#pragma once
#include	"component_base.h"
#include	"quad2d_component.h"

namespace Component {
	class Animation2d : public ComponentBase
	{
	private:
		bool isRepeat;					// アニメーションを繰り返すか
		unsigned int mFrameCnt;			// アニメーションが進む時にかかるフレーム
		unsigned int mCurrentFrame = 0;	// 現在のフレームカウント
		unsigned int mArray;			// 何列目のアニメーションか
		unsigned int mUvposU;			// 何番目のアニメーションか
		Quad2d* mQuad = nullptr;

	public:
		Animation2d();
		void Init() override;
		void Update()override;
		void ImguiDraw()override;
		void Uninit() override {};

		// アニメーション設定 (速度、縦何列目か)
		void SetAnim(int _frameCnt, int array);
	};
}