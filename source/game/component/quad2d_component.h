#pragma once
#include	"draw_component_base.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../../system/util/quad2d.h"

namespace Component {
	class Quad2d : public DrawComponentBase
	{
	private:
		Dix::sp<CQuad2D> mpQuad;
		DirectX::XMFLOAT4 mColor = DirectX::XMFLOAT4(1, 1, 1, 1);
		DrawType mDrawType;

		int mWidth;
		int mHeight;

	public:
		Quad2d();
		void Init() override;
		void Update()override;
		void Draw()override;
		void ImguiDraw()override;
		void Uninit() override;

		// ��`�̏������@(_texName�F�摜�̏ꏊ�Ɩ��O�@_color�F���_�J���[�@_u�F���̕������@_v�F�c�̕������@_z�F���`��T�C�Y)
		void SetInfo(std::string _texName, DirectX::XMFLOAT4 _color = DirectX::XMFLOAT4(1, 1, 1, 1), int _u = 1, int _v = 1, float _z = 0.0f);

		void SetColor(const DirectX::XMFLOAT4 &_color) { mColor = _color; }

		void SetUV(int u, int v);

		void SetDrawType(DrawType _drawType) { mDrawType = _drawType; };

	};
}

