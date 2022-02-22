#pragma once
#include		"gameobject.h"

enum class FadeType
{
	eIn,
	eOut,
};

class FadeScreen : public GameObject
{
private:
	float mAlpha;			// ‰æ‘œ‚Ìƒ¿’l
	bool mIsCompleted;		// “®ì‚ªŠ®—¹‚µ‚½‚©
	FadeType mCurrentType;

public:
	FadeScreen();
	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectImguiDraw()override {};
	void Uninit() override {};

	void SetFadeType(FadeType _type);
	bool GetIsCompleted() { return mIsCompleted; };
};