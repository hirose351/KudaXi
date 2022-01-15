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
	float mAlha;			// �摜�̃��l
	FadeType mCurrentType;
	bool mIsCompleted;		// ���삪����������
	bool mIsActive;

public:
	FadeScreen();
	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectDraw()override {};
	void ObjectImguiDraw()override {};
	void Uninit() override {};

	void SetFadeType(FadeType _type);
	bool GetIsCompleted() { return mIsCompleted; };
};