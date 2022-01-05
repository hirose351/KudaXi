#pragma once
#include	"gameobject.h"
#include	<vector>

// 切り替え方法
enum class ButtonTransition
{
	eImageSwap,
	eColorTint,
};

// ボタンの状態
enum class ButtonState
{
	eNomal = 0,		// 通常(選択可)
	eSelected,		// カーソル選択
	ePressed,		// 押された
	eDisabled,		// 選択不可
	MaxButtonState
};

// 描画開始地点
enum class StartPoint
{
	eLeftUp,	// 左上
	eRightUp,	// 右上
	eLeftDown,	// 左下
	eRightDown, // 右下
};

/*

選択されたときに大きくなる
トランスによって画像か色が変わる
画像が変わるならボタンの種類が行、変わる画像が列にする




*/





namespace myUI {
	class Button : public GameObject
	{
	private:
		ButtonState mButtonState;		// 切り替え方法
		ButtonTransition mTransition;
		XMFLOAT2 mNomalScale;
		XMFLOAT2 mSelectScale;

	public:
		Button();
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectDraw()override {};
		void ObjectImguiDraw()override {};
		void Uninit() override {};

		void SetTransition(ButtonTransition _trans)
		{
			mTransition = _trans;
		}
		void SetButtonState(ButtonState _state);
		void SetScale(XMFLOAT2 _nomalScale, XMFLOAT2 _selectScale)
		{
			mNomalScale = _nomalScale;
			mSelectScale = _selectScale;
		}
	};

	class ButtonGroup : public GameObject
	{
	private:
		std::vector<Button*> mButtonList;	// ボタンのリスト
		StartPoint mStartPoint;				// 開始地点
		DirectX::XMFLOAT2 mSpace;			// 余白の大きさ
		int mSelectNum;			// 選択されている番号
		unsigned int mArrayCnt;				// 横に並べる数
		unsigned int mPushTriggerFrame;		// 長押し時に進む数
		XMFLOAT4 mStateColor[(int)ButtonState::MaxButtonState];

		void SetSelectedNum(int _num);

	public:
		ButtonGroup();
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectDraw()override {};
		void ObjectImguiDraw()override;
		void Uninit() override {};

		// 初期値登録(btCount:サイコロの個数)
		void SetInitState(const char* _texName, int _divX, int _divY, int _arrayCnt, ButtonTransition _trans, XMFLOAT2 _startPos, XMFLOAT2 _nomalScale, XMFLOAT2 _selectScale, StartPoint _sP = StartPoint::eLeftUp);
		// 色登録
		void SetStateColor(ButtonState _state, const XMFLOAT4& _color)
		{
			mStateColor[(int)_state] = _color;
		}
	};
}