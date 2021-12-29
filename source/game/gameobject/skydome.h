#pragma once
#include	"gameobject.h"

class Skydome : public GameObject {
private:
	const float mRotationNum = 0.07f;
	DirectX::XMFLOAT4X4 mRotationMtx;	//âÒì]ópçsóÒ

public:
	enum Type {
		TITLE,
		GAME,
		CLEATE,
	};

	Skydome();

	void ObjectInit() override;
	void ObjectUpdate()override;
	void ObjectDraw()override;
	void ObjectImguiDraw()override {};
	void Uninit()override;

	void SetType(Type _type);
};