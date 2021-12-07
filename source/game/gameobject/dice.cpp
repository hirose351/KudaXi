#include	"dice.h"
#include	"../manager/dice_manager.h"
//#include	"../../system/util/XAudio2.h"

Dice::~Dice()
{
	//Finalize();
}

void Dice::ObjectInit()
{
	mDirection = Direction::eNeutral;
	StartUpPosition();
}

void Dice::ObjectUpdate()
{
	if (mSts == DICESTATUS::ROLL)
	{
		Roll();
	}
	else if (mSts == DICESTATUS::PUSH)
	{
		Push();
	}
	else if (mSts == DICESTATUS::UP || mSts == DICESTATUS::HALF_UP)
	{
		mTransform.AddPosition();
		mTransform.CreateMtx();
		mCrrentPushCnt++;
		if (mCrrentPushCnt >= mUpCnt)
		{
			mDirection = Direction::eNeutral;
			mSts = DICESTATUS::NORMAL;
		}
		else if (mCrrentPushCnt >= mUpCnt / 2)
		{
			mDirection = Direction::eNeutral;
			mSts = DICESTATUS::UP;
		}
	}
}

void Dice::ObjectDraw()
{
}

void Dice::Uninit()
{
}

void Dice::MoveDiceScale(Direction _direction)
{
	switch (_direction)
	{
	case Direction::eUp:
		mTransform.worldMtx._43 += DICE_SCALE;
		break;
	case Direction::eDown:
		mTransform.worldMtx._43 -= DICE_SCALE;
		break;
	case Direction::eLeft:
		mTransform.worldMtx._41 -= DICE_SCALE;
		break;
	case Direction::eRight:
		mTransform.worldMtx._41 += DICE_SCALE;
		break;
	}
}


bool Dice::SetPushAction(Direction _direction)
{
	if (!DiceManager::GetInstance()->CanDiceMove(this, _direction))
		return false;
	if (mDirection != Direction::eNeutral)
		return false;

	mTransform.move = 0;
	switch (_direction)
	{
	case Direction::eUp:
		mTransform.move.z = mPushPositionPerFrame;
		break;
	case Direction::eDown:
		mTransform.move.z = -mPushPositionPerFrame;
		break;
	case Direction::eLeft:
		mTransform.move.x = -mPushPositionPerFrame;
		break;
	case Direction::eRight:
		mTransform.move.x = mPushPositionPerFrame;
		break;
	}
	mSts = DICESTATUS::PUSH;
	mDirection = _direction;
	mCrrentPushCnt = 0;
	return true;
}

bool Dice::SetRollAction(Direction _direction)
{
	if (!DiceManager::GetInstance()->CanDiceMove(this, _direction))
		return false;
	if (mDirection != Direction::eNeutral)
		return false;

	mTransform.angle = 0;
	switch (_direction)
	{
	case Direction::eUp:
		mTransform.angle.x = mRotAnglePerFrame;
		break;
	case Direction::eDown:
		mTransform.angle.x = -mRotAnglePerFrame;
		break;
	case Direction::eLeft:
		mTransform.angle.z = mRotAnglePerFrame;
		break;
	case Direction::eRight:
		mTransform.angle.z = -mRotAnglePerFrame;
		break;
	}
	mSts = DICESTATUS::ROLL;
	mDirection = _direction;
	mCrrentRotCnt = 0;
	DX11MakeWorldMatrix(mMtxFrame, mTransform.angle, XMFLOAT3(0, 0, 0));
	mRotateStartPos = { mTransform.worldMtx._41, mTransform.worldMtx._42,mTransform.worldMtx._43 };
	return true;
}

void Dice::SetRollDirection(Direction _direction)
{
	//�j���[�g�����̎������L�[���͂�F�߂�
	if (mDirection == Direction::eNeutral)
	{
		mDirection = _direction;
		switch (_direction)
		{
		case Direction::eUp:
			mTransform.angle.x = mRotAnglePerFrame;
			break;
		case Direction::eDown:
			mTransform.angle.x = -mRotAnglePerFrame;
			break;
		case Direction::eLeft:
			mTransform.angle.z = mRotAnglePerFrame;
			break;
		case Direction::eRight:
			mTransform.angle.z = -mRotAnglePerFrame;
			break;
		}
		mCrrentRotCnt = 0;
		DX11MakeWorldMatrix(mMtxFrame, mTransform.angle, XMFLOAT3(0, 0, 0));
		//�J�n�ʒu��ۑ�
		mRotateStartPos = { mTransform.worldMtx._41, mTransform.worldMtx._42,mTransform.worldMtx._43 };
	}
}

void Dice::StartUpPosition()
{
	mTransform.move.y = mUpPositionPerFrame;
	mSts = DICESTATUS::UP;
}

void Dice::Push()
{
	mTransform.AddPosition();
	mTransform.CreateMtx();
	mCrrentPushCnt++;
	if (mCrrentPushCnt >= mMoveCnt)
	{
		mDirection = Direction::eNeutral;
		mSts = DICESTATUS::NORMAL;
	}
}

void Dice::Roll()
{
	//�s����쐬(���[���h�̎��𒆐S�ɉ�])
	DX11MtxMultiply(mTransform.worldMtx, mTransform.worldMtx, mMtxFrame);

	//���a���v�Z
	const static float radius = static_cast<float>(DICE_SCALE_HALF*sqrt(2));		// DICESCALE*���[�g2/2��DICESCALE/2.0f*���[�g2

	//45�x�����]�p�x�𑫂��Z
	float nowcenterposy = radius * sin(ToRad(45 + mRotAnglePerFrame * mCrrentRotCnt));

	//�ړ��ʂ̌v�Z
	Float3 pos = { mRotateStartPos.x,0.0f,mRotateStartPos.z };
	Float3 endpos;
	//�������v�Z
	float t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mMoveCnt);

	if (mDirection == Direction::eRight)
	{
		//�I���ʒu���v�Z
		endpos.x = mRotateStartPos.x + DICE_SCALE;
		//���`��Ԃ̎���X���W���v�Z
		pos.x = mRotateStartPos.x *(1.0f - t) + endpos.x*t;
	}

	if (mDirection == Direction::eLeft)
	{
		//�I���ʒu���v�Z
		endpos.x = mRotateStartPos.x - DICE_SCALE;
		//���`��Ԃ̎���X���W���v�Z
		pos.x = mRotateStartPos.x *(1.0f - t) + endpos.x*t;
	}

	if (mDirection == Direction::eUp)
	{
		//�I���ʒu���v�Z
		endpos.z = mRotateStartPos.z + DICE_SCALE;
		//���`��Ԃ̎���X���W���v�Z
		pos.z = mRotateStartPos.z *(1.0f - t) + endpos.z*t;
	}

	if (mDirection == Direction::eDown)
	{
		//�I���ʒu���v�Z
		endpos.z = mRotateStartPos.z - DICE_SCALE;
		//���`��Ԃ̎���X���W���v�Z
		pos.z = mRotateStartPos.z *(1.0f - t) + endpos.z*t;
	}

	//���_�̈ʒu��␳
	mTransform.worldMtx._41 = mTransform.position.x = pos.x;
	mTransform.worldMtx._42 = mTransform.position.y = nowcenterposy;
	mTransform.worldMtx._43 = mTransform.position.z = pos.z;

	//��]�����J�E���g�A�b�v
	mCrrentRotCnt++;

	// ��]���I������猳�̏�Ԃɖ߂�
	if (mCrrentRotCnt >= mMoveCnt)
	{
		//PlaySound(SOUND_LABEL_SE_DICE);
		mTransform.worldMtx._42 = mTransform.position.y = DICE_SCALE_HALF;
		mDirection = Direction::eNeutral;
		mSts = DICESTATUS::NORMAL;
		mCrrentRotCnt = 0;
		// ��]��̖ʂɐݒ�
		mTopDiceType = OverPlane();
		/// �ڂ��Ă���u���b�N�Ɩʂ��������`�F�b�N
		/// �X�e�b�v�����炷
	}
}

//�����������̖ʂ����
DICETYPE Dice::OverPlane() {
	Float3 underaxis(0.0f, -1.0f, 0.0f);
	Float3 axis;
	bool sts[6];
	int diceNum[6] = { 4,6,2,3,1,5 };
	float dot;

	// X�����o��
	axis = { mTransform.worldMtx._11, mTransform.worldMtx._12, mTransform.worldMtx._13 };
	// X���x�N�g���Ɖ������x�N�g���̓��ς��v�Z
	DX11Vec3Dot(dot, axis, underaxis);

	// ���̕����������Ēl�̔�r���s��
	sts[0] = floatcheck(dot, 7.0f, 0.001f);

	// Y���擾
	axis = { mTransform.worldMtx._21, mTransform.worldMtx._22, mTransform.worldMtx._23 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[1] = floatcheck(dot, 7.0f, 0.001f);
	// Z���擾
	axis = { mTransform.worldMtx._31, mTransform.worldMtx._32, mTransform.worldMtx._33 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[2] = floatcheck(dot, 7.0f, 0.001f);
	// -X���擾
	axis = { -mTransform.worldMtx._11, -mTransform.worldMtx._12, -mTransform.worldMtx._13 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[3] = floatcheck(dot, 7.0f, 0.001f);
	// -Y���擾
	axis = { -mTransform.worldMtx._21, -mTransform.worldMtx._22, -mTransform.worldMtx._23 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[4] = floatcheck(dot, 7.0f, 0.001f);
	// -Z���擾
	axis = { -mTransform.worldMtx._31, -mTransform.worldMtx._32, -mTransform.worldMtx._33 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[5] = floatcheck(dot, 7.0f, 0.001f);

	for (int i = 0; i < 6; i++)
	{
		if (sts[i])
		{
			return static_cast<DICETYPE>(i);
			break;
		}
	}
	return DICETYPE::APPLE;
}

void Dice::OnCollisionEnter(ComponentBase* _oher)
{
}

void Dice::OnCollisionStay(ComponentBase* _oher)
{
}

void Dice::OnCollisionExit(ComponentBase* _oher)
{
}