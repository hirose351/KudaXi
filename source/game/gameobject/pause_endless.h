#pragma once
#include	"gameobject.h"
#include	"ui_button.h"

namespace myUI {
	class PauseEndless : public GameObject
	{
	private:
		bool isPause;	// �|�[�Y��
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