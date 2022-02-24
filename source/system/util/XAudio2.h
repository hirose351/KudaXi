//=============================================================================
//
// �T�E���h���� [XAudio2.h]
//
//=============================================================================
#ifndef _XAUDIO2_H_
#define _XAUDIO2_H_

#include	<xaudio2.h>

// �T�E���h�t�@�C��
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		// �^�C�g��BGM
	SOUND_LABEL_BGM_GAME,			// �Q�[��BGM
	SOUND_LABEL_BGM_CREATE,			// �X�e�[�W�N���G�C�gBGM

	SOUND_LABEL_SE_SWITCH,			// �؂�ւ�SE
	SOUND_LABEL_SE_OK,				// ����SE
	SOUND_LABEL_SE_DICE,			// �T�C�R��SE
	SOUND_LABEL_SE_DICE_ALIGN,		// �T�C�R������SE

	SOUND_LABEL_SE_RETRY,			// �~�XSE
	SOUND_LABEL_SE_CLEAR,			// �N���ASE
	SOUND_LABEL_SE_START,			// �X�^�[�gSE
	SOUND_LABEL_SE_ONE,				// 1SE
	SOUND_LABEL_SE_TWO,				// 2SE
	SOUND_LABEL_SE_THREE,			// 3SE

	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(void);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void PauseSound(SOUND_LABEL label);
#endif
