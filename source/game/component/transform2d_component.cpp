#include "transform2d_component.h"

using namespace Component;

void Transform2d::CheckMoveMode()
{
	//�ړ����[�h���͂ɂ���ē�����ԁiForce�j�̎��͈ړ��ʂ�force�ŊǗ�����B
	//���̂Ƃ�direction��x,y���ꂼ��̒l��1�𒴂��Ă���ƈӐ}���Ȃ��ړ��ɂȂ��Ă��܂����߁Adirection�𐳋K�����Ă���

	if (moveMode == MoveMode::Volume)
		return;

	if (direction.x + direction.y > 1.41421356237)//�R�R�������́�2�Ȃ񂾂��ǂ߂�ǂ���������R����
		direction.Normalize();
}

void Transform2d::Update()
{
	CheckMoveMode();

	switch (moveMode)
	{
	case MoveMode::Volume:
		MoveByVolume();
		break;

	case MoveMode::Force:
		MoveByForce();
		break;
	}
}