#pragma once
#include "../gameobject/gameobject.h"

class GameObject;

class ComponentBase
{
protected:
	GameObject* mOwner;		// ���g�����L���Ă���Q�[���I�u�W�F�N�g

public:
	ComponentBase() { mOwner = nullptr; }
	virtual ~ComponentBase() { mOwner = nullptr; }

	GameObject* GetOwner() { return mOwner; }
	GameObject* SetOwner(GameObject* newowner) { mOwner = newowner;  return mOwner; }

	virtual void Draw() {}		//�`��i�V�ōX�V��������R���|�[�l���g�����܂��i��������Ȃ��j�̂ňȉ����i�������I�[�o�[�w�b�h�����������������A�R���p�C�����ǂ��ɂ����Ă����j
	virtual void Update() {}	//�X�V�i�V�ŕ`�悾������R���|�[�l���g�����܂��i��������Ȃ��j�̂ŁA�������z�֐��ɂ͂��Ȃ��ł����i�ꌩ�I�[�o�[�w�b�h�����������������A�R���p�C�����ǂ��ɂ����Ă����j
};