#pragma once
#include	"component_base.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../../system/util/quad2d.h"

namespace Component {
	class Quad2d : public ComponentBase
	{
	private:
		Dix::sp<CQuad2D> mpQuad;
		DirectX::XMFLOAT4 mColor = DirectX::XMFLOAT4(1, 1, 1, 1);
		bool mIsLateDraw;	// ��ŕ`�悷�邩

		int mWidth;
		int mHeight;

	public:
		Quad2d();
		void Init() override;
		void Update()override;
		void Draw()override;
		void LateDraw()override;
		void ImguiDraw()override;
		void Uninit() override;

		// ��`�̏������@(_texName�F�摜�̏ꏊ�Ɩ��O�@_color�F���_�J���[�@_u�F���̕������@_v�F�c�̕������@_z�F���`��T�C�Y)
		void SetInfo(const char* _texName, const DirectX::XMFLOAT4 &_color, int _u = 1, int _v = 1, float _z = 0.0f);

		void SetColor(const DirectX::XMFLOAT4 &_color)
		{
			mColor = _color;
		}

		void SetUV(int u, int v);

		void SetIsLateDraw(bool flg) { mIsLateDraw = flg; }
	};
}

