#include	"dice.h"
//#include	"../../system/util/XAudio2.h"

Dice::~Dice()
{
	//Finalize();
}

void Dice::Init()
{
	//�P�ʍs��
	DX11MtxIdentity(mScaleMtx);
	DX11MtxScale(7.0f, 7.0f, 7.0f, mScaleMtx);
	mMtx = mLocalMtx = mScaleMtx;
	// �����l������
	mAngle = { 0,0,0 };
	mDirection = DIRECTION::NEUTRAL;
	mCrrentRotCnt = 0;
}

void Dice::Update()
{
	//�j���[�g�����ȊO�ŗ����Ă��Ȃ��ꍇ
	if (mDirection != DIRECTION::NEUTRAL)
	{
		//�s����쐬(���[���h�̎��𒆐S�ɉ�])
		DX11MtxMultiply(mMtx, mMtx, mMtxFrame);

		//���a���v�Z
		const static float radius = static_cast<float>((DICESCALE / 2.0f)*sqrt(2));		// DICESCALE*���[�g2/2��DICESCALE/2.0f*���[�g2

		//45�x�����]�p�x�𑫂��Z
		float nowcenterposy = radius * sin(ToRad(45 + mRotAnglePerFrame * mCrrentRotCnt));

		//�ړ��ʂ̌v�Z
		Float3 pos = { mStartPos.x,0.0f,mStartPos.z };
		Float3 endpos;
		float t;

		if (mDirection == DIRECTION::RIGHT)
		{
			//�������v�Z
			t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mRotCnt);
			//�I���ʒu���v�Z
			endpos.x = mStartPos.x + DICESCALE;
			//���`��Ԃ̎���X���W���v�Z
			pos.x = mStartPos.x *(1.0f - t) + endpos.x*t;
		}

		if (mDirection == DIRECTION::LEFT)
		{
			//�������v�Z
			t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mRotCnt);
			//�I���ʒu���v�Z
			endpos.x = mStartPos.x - DICESCALE;
			//���`��Ԃ̎���X���W���v�Z
			pos.x = mStartPos.x *(1.0f - t) + endpos.x*t;
		}

		if (mDirection == DIRECTION::UP)
		{
			//�������v�Z
			t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mRotCnt);
			//�I���ʒu���v�Z
			endpos.z = mStartPos.z + DICESCALE;
			//���`��Ԃ̎���X���W���v�Z
			pos.z = mStartPos.z *(1.0f - t) + endpos.z*t;
		}

		if (mDirection == DIRECTION::DOWN)
		{
			//�������v�Z
			t = static_cast<float>(mCrrentRotCnt + 1) / static_cast<float>(mRotCnt);
			//�I���ʒu���v�Z
			endpos.z = mStartPos.z - DICESCALE;
			//���`��Ԃ̎���X���W���v�Z
			pos.z = mStartPos.z *(1.0f - t) + endpos.z*t;
		}

		//���_�̈ʒu��␳
		mMtx._41 = pos.x;
		mMtx._42 = nowcenterposy - DICESCALE / 2.0f;
		mMtx._43 = pos.z;

		//��]�����J�E���g�A�b�v
		mCrrentRotCnt++;

		// ��]���I������猳�̏�Ԃɖ߂�
		if (mCrrentRotCnt >= mRotCnt)
		{
			//PlaySound(SOUND_LABEL_SE_DICE);
			mDirection = DIRECTION::NEUTRAL;
			mCrrentRotCnt = 0;
			// ��]��̖ʂɐݒ�
			mTopDiceType = OverPlane();
			/// �ڂ��Ă���u���b�N�Ɩʂ��������`�F�b�N
			/// �X�e�b�v�����炷
		}
	}
}

void Dice::Draw()
{
	// ���f���`��
	mpModel->Draw(mMtx);
}

void Dice::Finalize()
{
	//mpModel->Uninit();
	//mpDownModel->Uninit();
}

void Dice::MoveDiceScale(DIRECTION _direction)
{
	switch (_direction)
	{
	case DIRECTION::UP:
		mMtx._43 += DICESCALE;
		break;
	case DIRECTION::DOWN:
		mMtx._43 -= DICESCALE;
		break;
	case DIRECTION::LEFT:
		mMtx._41 -= DICESCALE;
		break;
	case DIRECTION::RIGHT:
		mMtx._41 += DICESCALE;
		break;
	}
}

void Dice::OnCollisionEnter()
{
}

void Dice::OnCollisionStay()
{
}

void Dice::OnCollisionExit()
{
}

bool Dice::Push(DIRECTION _direction)
{
	return false;
}

bool Dice::Roll(DIRECTION _direction)
{
	//if (// ��]�s�Ȃ�)
	//{
	//return false;
	//}

	//else(// ��]�\�Ȃ�)�o

	// ���ψړ��ʁA�p�x
	Float3 trans, angle;

	switch (_direction)
	{
	case DIRECTION::UP:
		angle.x = 90;
		break;
	case DIRECTION::DOWN:
		angle.x = -90;
		break;
	case DIRECTION::LEFT:
		angle.z = 90;
		break;
	case DIRECTION::RIGHT:
		angle.z = -90;
		break;
	}
	DX11MakeWorldMatrix(mLocalMtx, angle, trans);
	DX11MtxMultiply(mMtx, mMtx, mLocalMtx);
	mTopDiceType = OverPlane();
	//}
	return true;
}

void Dice::SetRollDirection(DIRECTION _direction)
{
	// ���ψړ��ʁA�p�x��������
	Float3 trans, angle;

	//�j���[�g�����̎������L�[���͂�F�߂�
	if (mDirection == DIRECTION::NEUTRAL)
	{
		switch (_direction)
		{
		case DIRECTION::UP:
			mDirection = DIRECTION::UP;
			mAngle.x = mRotAnglePerFrame;
			break;
		case DIRECTION::DOWN:
			mDirection = DIRECTION::DOWN;
			mAngle.x = -mRotAnglePerFrame;
			break;
		case DIRECTION::LEFT:
			mDirection = DIRECTION::LEFT;
			mAngle.z = mRotAnglePerFrame;
			break;
		case DIRECTION::RIGHT:
			mDirection = DIRECTION::RIGHT;
			mAngle.z = -mRotAnglePerFrame;
			break;
		}
		mCrrentRotCnt = 0;
		DX11MakeWorldMatrix(mMtxFrame, mAngle, trans);
		//�J�n�ʒu��ۑ�
		mStartPos = { mMtx._41, mMtx._42,mMtx._43 };
	}
}

//�����������̖ʂ����
DICETYPE Dice::OverPlane() {
	Float3 underaxis = { 0,-1,0 };
	Float3 axis;
	bool sts[6];
	float dot;

	// X�����o��
	axis = { mMtx._11, mMtx._12, mMtx._13 };
	// X���x�N�g���Ɖ������x�N�g���̓��ς��v�Z
	DX11Vec3Dot(dot, axis, underaxis);

	// ���̕����������Ēl�̔�r���s��
	sts[0] = floatcheck(dot, 7.0f, 0.001f);

	// Y���擾
	axis = { mMtx._21, mMtx._22, mMtx._23 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[1] = floatcheck(dot, 7.0f, 0.001f);
	// Z���擾
	axis = { mMtx._31, mMtx._32, mMtx._33 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[2] = floatcheck(dot, 7.0f, 0.001f);
	// -X���擾
	axis = { -mMtx._11, -mMtx._12, -mMtx._13 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[3] = floatcheck(dot, 7.0f, 0.001f);
	// -Y���擾
	axis = { -mMtx._21, -mMtx._22, -mMtx._23 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[4] = floatcheck(dot, 7.0f, 0.001f);
	// -Z���擾
	axis = { -mMtx._31, -mMtx._32, -mMtx._33 };
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