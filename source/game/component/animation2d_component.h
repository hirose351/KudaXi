#pragma once
#include	"component_base.h"
#include	"quad2d_component.h"

namespace Component {
	class Animation2d : public ComponentBase
	{
	private:
		bool isRepeat;					// �A�j���[�V�������J��Ԃ���
		unsigned int mFrameCnt;			// �A�j���[�V�������i�ގ��ɂ�����t���[��
		unsigned int mCurrentFrame = 0;	// ���݂̃t���[���J�E���g
		unsigned int mArray;			// ����ڂ̃A�j���[�V������
		unsigned int mUvposU;			// ���Ԗڂ̃A�j���[�V������
		Quad2d* mQuad = nullptr;

	public:
		Animation2d();
		void Init() override;
		void Update()override;
		void ImguiDraw()override;
		void Uninit() override {};

		// �A�j���[�V�����ݒ� (���x�A�c����ڂ�)
		void SetAnim(int _frameCnt, int array);
	};
}