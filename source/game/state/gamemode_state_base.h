#pragma once
#include	"state_base.h"
#include	"gamemode_controller.h"
#include	<vector>
class GameModeController;
class GameObject;
class Player;

class GameModeBase :public StateBase
{
protected:
	GameModeController* mHolder;			// ������
	std::vector<Dix::wp<GameObject>> mModeObjList;	// ���[�h���ƂɎg�p����I�u�W�F�N�g(�R���X�g���N�^�Ő���,�A�N�e�B�u��ԊǗ�)
	Dix::wp<Player> mPlayer;

public:
	virtual ~GameModeBase() {};
	// ���삷��ϐ��o�^
	virtual void Start(GameModeController* _c) { mHolder = _c; };
};