#pragma once
#include	"draw_component_base.h"
#include	<d3d11.h>
#include	<vector>
#include	<wrl/client.h>
#include	"../../system/model/model_manager.h"

using Microsoft::WRL::ComPtr;

namespace Component {
	class Model : public DrawComponentBase
	{
	private:
		CModel* mpModel;

	public:
		Model();
		void Draw()override;
		void ImguiDraw()override;
		void Uninit() override;

		void SetModel(CModel* p) {
			mpModel = p;
		}

		// ピクセルシェーダーセット
		void SetPsShader(const char* _psfile) {
			mpModel->SetPsShader(_psfile);
		}
	};
}