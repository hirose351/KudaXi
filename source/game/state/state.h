#pragma once
#include	"../../system/util/dixsmartptr.h"
#include	"../component/player_controller.h"

enum Pstate
{
	eStop,	// ���ۂ͕K�v�Ȃ�
	eMove,
	ePush,
	eRoll,
};

namespace Component
{
	class PlayerController;
}

class State
{
protected:
	Dix::wp<Transform> mTramsform;
	Dix::wp<Component::PlayerController> mHolder;
public:
	virtual ~State() {};
	// ���삷��ϐ��o�^
	virtual void Start(Dix::wp<Component::PlayerController> _c);
	// ���s
	virtual void Exec() = 0;
	// State�ύX�O����
	virtual void BeforeChange() {}
	// State�ύX�㏈��
	virtual void AfterChange() {}
};