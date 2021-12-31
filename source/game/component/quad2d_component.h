#pragma once
#include	"component_base.h"
#include	"../../system/util/dixsmartptr.h"

class CQuad2D;
namespace Component {
	class Quad2d : public ComponentBase
	{
	private:
		Dix::sp<CQuad2D> mpQuad;

		int mWidth;
		int mHeight;

	public:
		Quad2d();
		void Init() override;
		void Update()override;
		void Draw()override;
		void ImguiDraw()override;
		void Uninit() override;

		// ��`�̏������@(width�F���`��T�C�Y�@height�F�c�`��T�C�Y�@tex_name�F�摜�̏ꏊ�Ɩ��O�@color�F���_�J���[�@u�F���̕������@v�F�c�̕������@z�F���`��T�C�Y)
		void SetInfo(XMFLOAT2 _scale, const char *tex_name, const XMFLOAT4 &color, int _u = 1, int _v = 1, float z = 0.0f);

		void SetScale(XMFLOAT2 _scale);
	};
}

