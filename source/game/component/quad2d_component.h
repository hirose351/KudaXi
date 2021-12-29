#pragma once
#include	"component_base.h"
#include	"../../system/util/dixsmartptr.h"

class Quad2D;
namespace Component {
	class Quad2dComponent : public ComponentBase
	{
	private:
		Dix::sp<Quad2D> mpQuad;

		int mWidth;
		int mHeight;

	public:
		void Init() override;
		void Update()override;
		void Draw()override;
		void ImguiDraw()override;
		void Uninit() override;

		// ��`�̏������@(width�F���`��T�C�Y�@height�F�c�`��T�C�Y�@tex_name�F�摜�̏ꏊ�Ɩ��O�@color�F���_�J���[�@u�F���̕������@v�F�c�̕������@z�F���`��T�C�Y)
		void SetInfo(float width, float height, const char *tex_name, const DirectX::XMFLOAT4 &color, int _u = 1, int _v = 1, float z = 0.0f);

		void SetScale(XMFLOAT2 _scale);
	};
}

