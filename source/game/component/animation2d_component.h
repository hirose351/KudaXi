#pragma once
#include	"component_base.h"

namespace Component {
	class Animation2d : public ComponentBase
	{
	private:

	public:
		Animation2d();
		void Init() override {};
		void Update()override {};
		void ImguiDraw()override {};
		void Uninit() override {};

		// �A�j���[�V�����ݒ�
		// ���x�A�c����ڂ�
		void SetAnim(float _speed, int array);
	};
}