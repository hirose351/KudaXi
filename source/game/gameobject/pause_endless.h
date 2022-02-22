#pragma once
#include	"gameobject.h"

namespace myUI {
	class ButtonGroup;
	class PauseEndless : public GameObject
	{
	private:
		bool mIsPause;	// É|Å[ÉYÇ©
		Dix::wp<myUI::ButtonGroup> mpButton;
		Dix::wp<GameObject> mpButtonBg;

	public:
		PauseEndless();
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectImguiDraw()override {};
		void Uninit() override {};
	};
}