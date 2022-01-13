#pragma once
#include	"../../system/util/uncopyable.h"
#include	"input_utility.h"

/*
コントローラーの情報とキーボードの情報どちらも取得して
必要な値を取り出す

UIで使う値、ゲーム中に使う値を取ってくる

UI
上下左右キー
決定
戻る
ポーズ

ゲーム中
移動方向
アクション(今のところなし)

*/

class InputManager : public Uncopyable
{
private:


public:
	static InputManager& GetInstance() {
		static InputManager Instance;
		return Instance;
	}

	bool GetState(InputMode _mode, int _action);
	bool GetStateTrigger(InputMode _mode, int _action);

	InputDirection GetDirection(InputMode _mode, int _action);
	InputDirection GetDirectionTrigger(InputMode _mode, int _action);
};

