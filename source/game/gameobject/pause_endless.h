#pragma once
#include	"gameobject.h"
#include	"ui_button.h"

namespace myUI {
	class PauseEndless : public GameObject
	{
	private:
		bool isPause;	// É|Å[ÉYÇ…Ç»Ç¡ÇΩÇ©
		myUI::ButtonGroup* mButton;

	public:
		PauseEndless();
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectImguiDraw()override {};
		void Uninit() override {};
	};
}