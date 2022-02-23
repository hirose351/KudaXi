#include "gameobject.h"

namespace Effect {
	class Score : public GameObject
	{
	private:
		float mAlha = 1.0f;							// ƒ¿’l
		Float3 mInitPos;

	public:
		Score();
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectImguiDraw()override {};
		void Uninit() override {};

		void SetInitPos(const Float3& _pos) { mInitPos = _pos; };
		void SetScoreNum(int _diceNum, int _diceCnt, int _chain);
	};
}
