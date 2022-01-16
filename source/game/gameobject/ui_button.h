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

// 並べ方
enum class ButtonArrangement
{
	eHorizontal,	// 横
	eVertical,		// 縦
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

使用時の登録順
色→初期情報→初期選択番号

*/

namespace myUI {
	class Button : public GameObject
	{
	private:
		ButtonState mButtonState;		// 切り替え方法
		ButtonTransition mTransition;
		DirectX::XMFLOAT2 mNomalScale;
		DirectX::XMFLOAT2 mSelectScale;

	public:
		Button();
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectImguiDraw()override {};
		void Uninit() override {};

		void SetTransition(ButtonTransition _trans)
		{
			mTransition = _trans;
		}
		void SetButtonState(ButtonState _state);
		void SetScale(DirectX::XMFLOAT2 _nomalScale, DirectX::XMFLOAT2 _selectScale)
		{
			mNomalScale = _nomalScale;
			mSelectScale = _selectScale;
		}
	};

	class ButtonGroup : public GameObject
	{
	private:
		std::vector<Button*> mButtonList;		// ボタンのリスト
		StartPoint mStartPoint;					// 開始地点
		DirectX::XMFLOAT2 mSpace;				// 余白の大きさ
		int mSelectNum;							// 選択されている番号
		unsigned int mArrayCnt;					// 横に並べる数
		unsigned int mPressingTriggerFrame;		// 長押し時に進む数
		DirectX::XMFLOAT4 mStateColor[(int)ButtonState::MaxButtonState] = { DirectX::XMFLOAT4(0.5f,0.5f,0.5f,1.0f), DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f),DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f) };
		bool isPressed = false;		// ボタンが押されたか
		ButtonArrangement mArrangement;
		DirectX::XMFLOAT2 mNomalScale;
		Float3 mBeforePos;

		// 選択された数の処理
		void SetSelectedNum(int _num);

		void SetButtonPosition();
	public:
		ButtonGroup();
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectImguiDraw()override;
		void Uninit() override {};

		// 色登録
		void SetStateColor(ButtonState _state, const DirectX::XMFLOAT4& _color)
		{
			mStateColor[(int)_state] = _color;
		}

		// 初期値登録(btCount:サイコロの個数)
		void SetInitState(const char* _texName, int _divX, int _divY, int _arrayCnt, ButtonTransition _trans, DirectX::XMFLOAT2 _space, DirectX::XMFLOAT2 _nomalScale, DirectX::XMFLOAT2 _selectScale, ButtonArrangement _ar = ButtonArrangement::eHorizontal, StartPoint _sP = StartPoint::eLeftUp);

		// 初期選択番号登録
		void SetInitSelectNum(int _num);

		// 
		void SetPosition(int _num, DirectX::XMFLOAT2 _pos);

		// 選択されている番号を返す
		int GetSelectNum() { return mSelectNum; }

		// 押されたかを返す
		bool GetIsPressed() { return isPressed; }
	};
}