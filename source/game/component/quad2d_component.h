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

		// 矩形の初期化　(width：横描画サイズ　height：縦描画サイズ　tex_name：画像の場所と名前　color：頂点カラー　u：横の分割数　v：縦の分割数　z：奥描画サイズ)
		void SetInfo(XMFLOAT2 _scale, const char *tex_name, const XMFLOAT4 &color, int _u = 1, int _v = 1, float z = 0.0f);

		void SetScale(XMFLOAT2 _scale);
	};
}

