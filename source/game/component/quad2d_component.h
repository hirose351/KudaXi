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

		// ��`�̏������@(_scale�F�傫���@_texName�F�摜�̏ꏊ�Ɩ��O�@_color�F���_�J���[�@_u�F���̕������@_v�F�c�̕������@_z�F���`��T�C�Y)
		void SetInfo(XMFLOAT2 _scale, const char* _texName, const XMFLOAT4 &_color, int _u = 1, int _v = 1, float _z = 0.0f);

		void SetScale(XMFLOAT2 _scale);
	};
}

