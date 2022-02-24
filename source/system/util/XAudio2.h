//=============================================================================
//
// サウンド処理 [XAudio2.h]
//
//=============================================================================
#ifndef _XAUDIO2_H_
#define _XAUDIO2_H_

#include	<xaudio2.h>

// サウンドファイル
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		// タイトルBGM
	SOUND_LABEL_BGM_GAME,			// ゲームBGM
	SOUND_LABEL_BGM_CREATE,			// ステージクリエイトBGM

	SOUND_LABEL_SE_SWITCH,			// 切り替えSE
	SOUND_LABEL_SE_OK,				// 決定SE
	SOUND_LABEL_SE_DICE,			// サイコロSE
	SOUND_LABEL_SE_DICE_ALIGN,		// サイコロ揃うSE

	SOUND_LABEL_SE_RETRY,			// ミスSE
	SOUND_LABEL_SE_CLEAR,			// クリアSE
	SOUND_LABEL_SE_START,			// スタートSE
	SOUND_LABEL_SE_ONE,				// 1SE
	SOUND_LABEL_SE_TWO,				// 2SE
	SOUND_LABEL_SE_THREE,			// 3SE

	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(void);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void PauseSound(SOUND_LABEL label);
#endif
