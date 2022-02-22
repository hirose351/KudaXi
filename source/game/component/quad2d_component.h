#pragma once
#include	"draw_component_base.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../../system/util/quad2d.h"

namespace Component {
	class Quad2d : public DrawComponentBase
	{
	private:
		std::vector<INT2> mDrawUvList;	// �A�����ĕ`�悷��ꍇ��UV�ʒu���X�g
		DirectX::XMFLOAT4 mColor = DirectX::XMFLOAT4(1, 1, 1, 1);
		Dix::sp<CQuad2D> mpQuad;
		DrawType mDrawType;
		INT2 mUvPos;					// Uv�ʒu
		INT2 mTexUv;					// �e�N�X�`���̕�����

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
		void SetInfo(std::string _texName, const DirectX::XMFLOAT4& _color, int _u = 1, int _v = 1, float _z = 0.0f);

		void SetColor(const DirectX::XMFLOAT4 &_color);

		void SetUvPos(INT2 _uv);
		INT2 GetUvPos() { return mUvPos; };
		INT2 GetTexUv() { return mTexUv; };
		void SetDrawUv(INT2 _uv);

		void SetDrawType(DrawType _drawType) { mDrawType = _drawType; };
	};
}

