#pragma once
#include	"gameobject.h"
#include	"effect_util.h"

namespace Effect {
	class Fruit : public GameObject
	{
	private:
		float mAlha = 1.0f;							// —‹ƒ¿’l
		Float3 mInitPos;

		std::vector<PARTICLE> mParticle;
		XMFLOAT3 mCenterPos;	// ¶¬‚³‚ê‚é‚Æ‚«‚Ì’†SÀ•W
	public:
		Fruit();
		~Fruit();
		void ObjectInit() override;
		void ObjectUpdate()override;
		void ObjectDraw()override;
		void ObjectImguiDraw()override {};
		void Uninit() override {};

		void SetInit(const XMFLOAT3 &_pos, int _uvnum);
	};
}

