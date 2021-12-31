#pragma once
#include	"component_base.h"
#include	<d3d11.h>
#include	<vector>
#include	<wrl/client.h>
#include	"../../system/model/ModelMgr.h"

using Microsoft::WRL::ComPtr;

namespace Component {
	class Model : public ComponentBase
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
	};
}