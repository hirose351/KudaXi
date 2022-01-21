#pragma once
#include "component_base.h"

namespace Component {
	class MapPos : public ComponentBase
	{
	private:
		INT2 mPos;

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
		void AddMapPos(INT2 _pos) { mPos += _pos; }
	};
}