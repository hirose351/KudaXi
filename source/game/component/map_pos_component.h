#pragma once
#include	"component_base.h"
#include	"../data/stage_data.h"

class StageData;
namespace Component {
	class MapPos : public ComponentBase
	{
	private:
		INT2 mPos;
		StageData mCurrentStageData;

		void SetPos();

	public:
		MapPos();

		void Awake()override {};
		void Init()override;
		void Update()override;
		void ImguiDraw()override;
		void Uninit()override {};

		/// アクセサ
		INT2 GetMapPos() { return mPos; }
		void SetMapPos(INT2 _pos) { mPos = _pos; }
		void SetMapPosMove(INT2 _pos);
		bool AddMapPos(INT2 _pos);
	};
}