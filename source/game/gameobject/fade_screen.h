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
	float mAlpha;			// �摜�̃��l
	bool mIsCompleted;		// ���삪����������
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