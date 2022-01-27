#include	"dice.h"
#include	"../manager/dice_manager.h"
//#include	"../../system/util/XAudio2.h"
#include	"../../system/util/easing.h"
#include	"effect_thunder.h"
#include	"../../system/model/ModelMgr.h"
#include	"../component/allcomponents.h"
#include	"../manager/scene_manager.h"

Dice::Dice() :GameObject(("Dice"), ObjectType::eDice, true)
{
	bool sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/dice/Dice.fbx",
		"shader/vs.hlsl", "shader/toonps.hlsl",
		"assets/model/dice/");
	if (!sts)
	{
		MessageBox(nullptr, "Dice���f�� �ǂݍ��݃G���[", "error", MB_OK);
	}
	AddComponent<Component::Model>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/dice/Dice.fbx"));
	AddComponent<Component::Collision>()->SetInitState(ObjectTag::eDice, Float3(0, 0, 0), Float3(DICE_SCALE_HALF), DirectX::XMFLOAT4(1, 1, 1, 0.5f));
}

Dice::~Dice()
{
}

void Dice::ObjectInit()
{
	if (mSts == DiceStatus::eCreate)
		return;

	mDirection = Direction::eNeutral;
	SetStartUpPosition();
	SetOverPlane();
	Dix::sp<Effect::Thunder> effect;
	effect.SetPtr(new Effect::Thunder);
	SceneManager::GetInstance()->GetScene(mSceneKey)->AddGameObject(effect);

	Float3 pos = { mTransform->position.x , 0, mTransform->position.z - DICE_SCALE_HALF };
	effect->SetInitPos(pos);
	effect->Init();
}

void Dice::ObjectUpdate()
{
	switch (mSts)
	{
	case DiceStatus::eUp:
		GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 1, 1, 0));
		Up();
		break;
	case DiceStatus::eHalfUp:
		GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 1, 1, 0.5f));
		Up();
		break;
	case DiceStatus::eRoll:
		GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 1, 1, 0));
		Roll();
		break;
	case DiceStatus::ePush:
		GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 1, 1, 0));
		Push();
		break;
	case DiceStatus::eDown:
		GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 0, 0, 0.5f));
		Down();
		break;
	case DiceStatus::eHalfDown:
		GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 0, 0, 0.3f));
		Down();
		break;
	case DiceStatus::eNormal:
		GetComponent<Component::Collision>()->SetColor(XMFLOAT4(1, 1, 1, 0));
		break;
	}
}

void Dice::Uninit()
{
}

void Dice::SetHappyOne()
{
	// �o�ڂ̐���1�łȂ���ΕԂ�
	if (mTopDiceTypeNum != 1)
		return;
	// 1�ł���Η��Ƃ�
	SetDownPosition();
}

void Dice::MoveDiceScale(Direction _direction)
{
	switch (_direction)
	{
	case Direction::eUp:
		mTransform->worldMtx._43 += DICE_SCALE;
		break;
	case Direction::eDown:
		mTransform->worldMtx._43 -= DICE_SCALE;
		break;
	case Direction::eLeft:
		mTransform->worldMtx._41 -= DICE_SCALE;
		break;
	case Direction::eRight:
		mTransform->worldMtx._41 += DICE_SCALE;
		break;
	}
}

bool Dice::SetPushAction(Direction _direction)
{
	if (mSts != DiceStatus::eNormal)
		return false;
	if (!DiceManager::GetInstance()->CanDiceMove(this, _direction))
		return false;

	mTransform->move = 0;
	switch (_direction)
	{
	case Direction::eUp:
		mTransform->move.z = mPushPositionPerFrame;
		break;
	case Direction::eDown:
		mTransform->move.z = -mPushPositionPerFrame;
		break;
	case Direction::eLeft:
		mTransform->move.x = -mPushPositionPerFrame;
		break;
	case Direction::eRight:
		mTransform->move.x = mPushPositionPerFrame;
		break;
	}
	mSts = DiceStatus::ePush;
	mDirection = _direction;
	mCrrentPushCnt = 0;
	return true;
}

bool Dice::SetRollAction(Direction _direction)
{
	if (mSts != DiceStatus::eNormal)
		return false;
	if (!DiceManager::GetInstance()->CanDiceMove(this, _direction))
		return false;
	mTransform->angle = 0;
	mBeforeFramePos = 0;
	mBeforeFrameAng = 0;
	switch (_direction)
	{
	case Direction::eUp:
		mTransform->angle.x = 90.0f;
		break;
	case Direction::eDown:
		mTransform->angle.x = -90.0f;
		break;
	case Direction::eLeft:
		mTransform->angle.z = 90.0f;
		break;
	case Direction::eRight:
		mTransform->angle.z = -90.0f;
		break;
	}
	mSts = DiceStatus::eRoll;
	mDirection = _direction;
	mCrrentRotCnt = 0;
	mStartMtx = mTransform->worldMtx;
	DX11MakeWorldMatrix(mTargetMtx, mTransform->angle, XMFLOAT3(0, 0, 0));
	// �s����쐬(���[���h�̎��𒆐S�ɉ�])
	DX11MtxMultiply(mTargetMtx, mTransform->worldMtx, mTargetMtx);
	mRotateStartPos = { mTransform->worldMtx._41, mTransform->worldMtx._42,mTransform->worldMtx._43 };
	return true;
}

bool Dice::CheckDiceDirection(Direction _direction)
{
	return DiceManager::GetInstance()->CanDiceMoveCheak(this, _direction);
}

void Dice::SetRollDirection(Direction _direction)
{
	//�j���[�g�����̎������L�[���͂�F�߂�
	if (mDirection == Direction::eNeutral)
	{
		mDirection = _direction;

		//switch (_direction)
		//{
		//case Direction::eUp:
		//	mTransform.angle.x = mRotAnglePerFrame;
		//	break;
		//case Direction::eDown:
		//	mTransform.angle.x = -mRotAnglePerFrame;
		//	break;
		//case Direction::eLeft:
		//	mTransform.angle.z = mRotAnglePerFrame;
		//	break;
		//case Direction::eRight:
		//	mTransform.angle.z = -mRotAnglePerFrame;
		//	break;
		//}
		//mCrrentRotCnt = 0;
		//DX11MakeWorldMatrix(mMtxFrame, mTransform.angle, XMFLOAT3(0, 0, 0));
		//�J�n�ʒu��ۑ�
		mRotateStartPos = { mTransform->worldMtx._41, mTransform->worldMtx._42,mTransform->worldMtx._43 };
	}
}

void Dice::SetStartUpPosition()
{
	mTransform->move.y = mUpPositionPerFrame;
	mSts = DiceStatus::eHalfUp;
}

void Dice::SetDownPosition()
{
	mTransform->move = 0;
	mTransform->move.y = -mUpPositionPerFrame;
	mSts = DiceStatus::eDown;
}

void Dice::Push()
{
	mTransform->MovePosition();
	mCrrentPushCnt++;
	if (mCrrentPushCnt >= mMoveCnt)
	{
		mDirection = Direction::eNeutral;
		mSts = DiceStatus::eNormal;
		/// �ڂ��Ă���u���b�N�Ɩʂ��������`�F�b�N
		DiceManager::GetInstance()->CheckAligned(this);
		/// �X�e�b�v�����炷
	}
}

void Dice::Roll()
{
	// ���a���v�Z
	const static float radius = static_cast<float>(DICE_SCALE_HALF*sqrt(2));		// DICESCALE*���[�g2/2��DICESCALE/2.0f*���[�g2

	// �������v�Z
	float frameAngle = EasingProcess::CircOut(static_cast<float>(mCrrentRotCnt), static_cast<float>(mMoveCnt + 1), 0.0f, 90.0f);
	float framePos = EasingProcess::CircOut(static_cast<float>(mCrrentRotCnt), static_cast<float>(mMoveCnt + 1), 0.0f, DICE_SCALE);

	// 45�x�����]�p�x�𑫂��Z
	float nowcenterposy = radius * sin(ToRad(45 + frameAngle));
	// �ړ��ʂ̌v�Z
	Float3 pos = { mRotateStartPos.x,nowcenterposy,mRotateStartPos.z };
	Float3 endpos = pos;

	switch (mDirection)
	{
	case Direction::eUp:
		mTransform->angle.x = frameAngle;
		//  �I���ʒu���v�Z
		endpos.z = mRotateStartPos.z + DICE_SCALE;
		pos.z = mRotateStartPos.z + framePos;
		break;
	case Direction::eDown:
		mTransform->angle.x = -frameAngle;
		endpos.z = mRotateStartPos.z - DICE_SCALE;
		pos.z = mRotateStartPos.z - framePos;
		break;
	case Direction::eLeft:
		mTransform->angle.z = frameAngle;
		endpos.x = mRotateStartPos.x - DICE_SCALE;
		pos.x = mRotateStartPos.x - framePos;
		break;
	case Direction::eRight:
		mTransform->angle.z = -frameAngle;
		endpos.x = mRotateStartPos.x + DICE_SCALE;
		pos.x = mRotateStartPos.x + framePos;
		break;
	}

	mBeforeFramePos = framePos;
	mBeforeFrameAng = frameAngle;

	XMFLOAT4X4 rotatinMtx;

	DX11MakeWorldMatrix(rotatinMtx, mTransform->angle, XMFLOAT3(0, 0, 0));
	// �s����쐬(���[���h�̎��𒆐S�ɉ�])
	DX11MtxMultiply(mTransform->worldMtx, mStartMtx, rotatinMtx);
	// ���_�̈ʒu��␳
	mTransform->SetPositionXYZ(pos);

	// ��]���I������猳�̏�Ԃɖ߂�
	if (mCrrentRotCnt >= mMoveCnt)
	{
		mTransform->worldMtx = mTargetMtx;
		endpos.y = DICE_SCALE_HALF;
		mTransform->SetPositionXYZ(endpos);
		mDirection = Direction::eNeutral;
		mSts = DiceStatus::eNormal;
		// ��]��̖ʂɐݒ�
		SetOverPlane();
		/// �ڂ��Ă���u���b�N�Ɩʂ��������`�F�b�N
		DiceManager::GetInstance()->CheckAligned(this);
		/// Todo:�X�e�b�v�����炷
	}
	// ��]�����J�E���g�A�b�v
	mCrrentRotCnt++;
}

void Dice::Up()
{
	mTransform->MovePosition();
	mCrrentPushCnt++;
	if (mCrrentPushCnt >= mUpCnt)
	{
		mDirection = Direction::eNeutral;
		mSts = DiceStatus::eNormal;
	}
	else if (mCrrentPushCnt >= mUpCnt / 2)
	{
		mDirection = Direction::eNeutral;
		mSts = DiceStatus::eUp;
	}
}

void Dice::Down()
{
	mTransform->MovePosition();
	if (mTransform->position.y < -DICE_SCALE_HALF)
	{
		DiceManager::GetInstance()->SetRemoveDice(GetObjectID());
	}
	else if (mTransform->position.y < 0)
	{
		mDirection = Direction::eNeutral;
		mSts = DiceStatus::eHalfDown;
	}
}

void Dice::SetOverPlane() {
	Float3 underaxis(0.0f, -1.0f, 0.0f);
	Float3 axis;
	bool sts[6];
	float range = 0.001f;
	int diceNum[6] = { 4,6,2,3,1,5 };
	float dot;

	// X�����o��
	axis = { mTransform->worldMtx._11, mTransform->worldMtx._12, mTransform->worldMtx._13 };
	// X���x�N�g���Ɖ������x�N�g���̓��ς��v�Z
	DX11Vec3Dot(dot, axis, underaxis);

	// ���̕����������Ēl�̔�r���s��
	sts[0] = floatcheck(dot, 1.0f, range);
	// Y���擾
	axis = { mTransform->worldMtx._21, mTransform->worldMtx._22, mTransform->worldMtx._23 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[1] = floatcheck(dot, 1.0f, range);
	// Z���擾
	axis = { mTransform->worldMtx._31, mTransform->worldMtx._32, mTransform->worldMtx._33 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[2] = floatcheck(dot, 1.0f, range);
	// -X���擾
	axis = { -mTransform->worldMtx._11, -mTransform->worldMtx._12, -mTransform->worldMtx._13 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[3] = floatcheck(dot, 1.0f, range);
	// -Y���擾
	axis = { -mTransform->worldMtx._21, -mTransform->worldMtx._22, -mTransform->worldMtx._23 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[4] = floatcheck(dot, 1.0f, range);
	// -Z���擾
	axis = { -mTransform->worldMtx._31, -mTransform->worldMtx._32, -mTransform->worldMtx._33 };
	DX11Vec3Dot(dot, axis, underaxis);
	sts[5] = floatcheck(dot, 1.0f, range);

	for (int i = 0; i < 6; i++)
	{
		if (sts[i])
		{
			mTopDiceTypeFruit = static_cast<DiceFruit>(i);
			mTopDiceTypeNum = diceNum[i];
			break;
		}
	}
}