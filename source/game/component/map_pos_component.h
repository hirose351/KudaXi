#pragma once
#include	"component_base.h"

class StageData;
namespace Component {
	class MapPos : public ComponentBase
	{
	private:
		INT2 mPos;
		Dix::wp<StageData> mpCurrentStageData;

		void SetPos();

	public:
		MapPos();

		void Awake()override {};
		void Init()override;
		void Update()override;
		void ImguiDraw()override;
		void Uninit()override {};

		/// ?A?N?Z?T
		INT2 GetMapPos() { return mPos; }
		void SetMapPos(INT2 _pos) { mPos = _pos; }
		void SetMapPosMove(INT2 _pos);
		bool AddMapPos(INT2 _pos);
	};
}