#pragma once
#include	"draw_component_base.h"
#include	"../../system/util/dixsmartptr.h"
#include	"../../system/util/quad2d.h"

namespace Component {
	class Quad2d : public DrawComponentBase
	{
	private:
		std::vector<INT2> mDrawUvList;	// 連続して描画する場合のUV位置リスト
		DirectX::XMFLOAT4 mColor = DirectX::XMFLOAT4(1, 1, 1, 1);
		Dix::sp<CQuad2D> mpQuad;
		DrawType mDrawType;
		INT2 mUvPos;					// Uv位置
		INT2 mTexUv;					// テクスチャの分割数

		int mWidth;
		int mHeight;

	public:
		Quad2d();
		void Init() override;
		void Update()override;
		void Draw()override;
		void ImguiDraw()override;
		void Uninit() override;

		// 矩形の初期化　(_texName：画像の場所と名前　_color：頂点カラー　_u：横の分割数　_v：縦の分割数　_z：奥描画サイズ)
		void SetInfo(std::string _texName, const DirectX::XMFLOAT4& _color, int _u = 1, int _v = 1, float _z = 0.0f);

		void SetColor(const DirectX::XMFLOAT4 &_color);

		void SetUvPos(INT2 _uv);
		INT2 GetUvPos() { return mUvPos; };
		INT2 GetTexUv() { return mTexUv; };
		void SetDrawUv(INT2 _uv);

		void SetDrawType(DrawType _drawType) { mDrawType = _drawType; };
	};
}

