#pragma once
#include	"gameobject.h"
#include	"effect_util.h"

namespace Effect {
	class Fruit : public GameObject
	{
	private:
		float mAlha = 1.0f;					// α値
		Float3 mInitPos;

		std::vector<PARTICLE> mParticle;
		Float3 mCenterPos;					// 生成されるときの中心座標

	public:
		Fruit();
		~Fruit();

		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectImguiDraw()override {};
		void Uninit() override {};

		void SetInit(const Float3& _pos, int _uvnum);
	};
}