#pragma once
#include	"../../system/util/uncopyable.h"
#include	"input_utility.h"

/*
�R���g���[���[�̏��ƃL�[�{�[�h�̏��ǂ�����擾����
�K�v�Ȓl�����o��

UI�Ŏg���l�A�Q�[�����Ɏg���l������Ă���

UI
�㉺���E�L�[
����
�߂�
�|�[�Y

�Q�[����
�ړ�����
�A�N�V����(���̂Ƃ���Ȃ�)

*/

class InputManager : public Uncopyable
{
private:


public:
	static InputManager& GetInstance() {
		static InputManager Instance;
		return Instance;
	}

	bool GetState(InputMode _mode, int _action);
	bool GetStateTrigger(InputMode _mode, int _action);

	InputDirection GetDirection(InputMode _mode, int _action);
	InputDirection GetDirectionTrigger(InputMode _mode, int _action);
};

