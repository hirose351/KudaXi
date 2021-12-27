#pragma once
#include	"component_base.h"
#include	"../../system/util/dixsmartptr.h"

class Quad2D;
namespace Component {
	class Quad2dComponent : public ComponentBase
	{
	private:
		Dix::sp<Quad2D> mpQuad;

	public:
		void Init() override;
		void Update()override;
		void Draw()override;
		void ImguiDraw()override;
		void Uninit() override;

		// 矩形の初期化　(width：横描画サイズ　height：縦描画サイズ　tex_name：画像の場所と名前　color：頂点カラー　u：横の分割数　v：縦の分割数　z：奥描画サイズ)
		void SetInfo(int width, int height, const char *tex_name, const DirectX::XMFLOAT4 &color, int _u = 1, int _v = 1, float z = 0.0f);
	};
}

