//=============================================================================
//
// サウンド処理 [XAudio2.cpp]
//
//=============================================================================

#include	"XAudio2.h"

// パラメータ構造体
typedef struct
{
	LPCSTR filename;	// 音声ファイルまでのパスを設定
	bool bLoop;			// trueでループ。通常BGMはture、SEはfalse。
} PARAM;

PARAM gParam[SOUND_LABEL_MAX] =
{
	// BGM
	{"assets/audio/bgm/title.wav", true},		// タイトルBGM（ループさせるのでtrue設定）
	{"assets/audio/bgm/game.wav", true},		// ゲームBGM
	{"assets/audio/bgm/create.wav", true},		// ステージクリエイトBGM
	// SE
	{"assets/audio/se/switch.wav", false},  	// 切り替えSE（ループしないのでfalse設定）
	{"assets/audio/se/ok.wav", false},			// 決定SE
	{"assets/audio/se/dice.wav", false},		// サイコロSE

	{"assets/audio/se/miss.wav", false},		// ミスSE
	{"assets/audio/se/yeah.wav", false},		// クリアSE
	{"assets/audio/se/start.wav", false},		// スタートSE
	{"assets/audio/se/one.wav", false},			// 1SE
	{"assets/audio/se/two.wav", false},			// 2SE
	{"assets/audio/se/three.wav", false},		// 3SE
};

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif
#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif


//-----------------------------------------------------------------
//    グローバル変数
//-----------------------------------------------------------------
IXAudio2               *gpXAudio2 = NULL;
IXAudio2MasteringVoice *gpMasteringVoice = NULL;
IXAudio2SourceVoice    *gpSourceVoice[SOUND_LABEL_MAX];

WAVEFORMATEXTENSIBLE	gWfx[SOUND_LABEL_MAX];			// WAVフォーマット
XAUDIO2_BUFFER			gBuffer[SOUND_LABEL_MAX];
BYTE					*gDataBuffer[SOUND_LABEL_MAX];

//-----------------------------------------------------------------
//    プロトタイプ宣言
//-----------------------------------------------------------------
HRESULT FindChunk(HANDLE, DWORD, DWORD&, DWORD&);
HRESULT ReadChunkData(HANDLE, void*, DWORD, DWORD);

//=============================================================================
// 初期化
//=============================================================================
HRESULT InitSound()
{
	HRESULT      hr;

	HANDLE               hFile;
	DWORD                dwChunkSize;
	DWORD                dwChunkPosition;
	DWORD                filetype;

	// ＣＯＭの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	/**** Create XAudio2 ****/
	hr = XAudio2Create(&gpXAudio2, 0);		// 第二引数は､動作フラグ デバッグモードの指定(現在は未使用なので0にする)
	//hr=XAudio2Create(&g_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);		// 第三引数は、windowsでは無視
	if (FAILED(hr))
	{
		CoUninitialize();
		return -1;
	}

	/**** Create Mastering Voice ****/
	hr = gpXAudio2->CreateMasteringVoice(&gpMasteringVoice);			// 今回はＰＣのデフォルト設定に任せている
	/*, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL*/		// 本当６個の引数を持っている
	if (FAILED(hr))
	{
		if (gpXAudio2)	gpXAudio2->Release();
		CoUninitialize();
		return -1;
	}

	/**** Initalize Sound ****/
	for (int i = 0; i < SOUND_LABEL_MAX; i++)
	{
		memset(&gWfx[i], 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&gBuffer[i], 0, sizeof(XAUDIO2_BUFFER));

		hFile = CreateFile(gParam[i].filename, GENERIC_READ, FILE_SHARE_READ, NULL,
						   OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}

		//check the file type, should be fourccWAVE or 'XWMA'
		FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
		ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
		if (filetype != fourccWAVE)		return S_FALSE;

		FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
		ReadChunkData(hFile, &gWfx[i], dwChunkSize, dwChunkPosition);

		//fill out the audio data buffer with the contents of the fourccDATA chunk
		FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
		gDataBuffer[i] = new BYTE[dwChunkSize];
		ReadChunkData(hFile, gDataBuffer[i], dwChunkSize, dwChunkPosition);

		CloseHandle(hFile);

		// 	サブミットボイスで利用するサブミットバッファの設定
		gBuffer[i].AudioBytes = dwChunkSize;
		gBuffer[i].pAudioData = gDataBuffer[i];
		gBuffer[i].Flags = XAUDIO2_END_OF_STREAM;
		if (gParam[i].bLoop)
			gBuffer[i].LoopCount = XAUDIO2_LOOP_INFINITE;
		else
			gBuffer[i].LoopCount = 0;

		gpXAudio2->CreateSourceVoice(&gpSourceVoice[i], &(gWfx[i].Format));

		if (gParam[i].bLoop)
			gpSourceVoice[i]->SetVolume(0.1f);
		else
			gpSourceVoice[i]->SetVolume(0.3f);
	}

	return hr;
}

//=============================================================================
// 開放処理
//=============================================================================
void UninitSound(void)
{
	for (int i = 0; i < SOUND_LABEL_MAX; i++)
	{
		if (gpSourceVoice[i])
		{
			gpSourceVoice[i]->Stop(0);
			gpSourceVoice[i]->FlushSourceBuffers();
			gpSourceVoice[i]->DestroyVoice();			// オーディオグラフからソースボイスを削除
			delete[]  gDataBuffer[i];
		}
	}

	gpMasteringVoice->DestroyVoice();

	if (gpXAudio2) gpXAudio2->Release();

	// ＣＯＭの破棄
	CoUninitialize();
}

//=============================================================================
// 再生
//=============================================================================
void PlaySound(SOUND_LABEL label)
{
	// ソースボイス作成
	//g_pXAudio2->CreateSourceVoice(&(g_pSourceVoice[(int)label]), &(g_wfx[(int)label].Format));
	gpSourceVoice[(int)label]->SubmitSourceBuffer(&(gBuffer[(int)label]));	// ボイスキューに新しいオーディオバッファーを追加

	// 再生
	gpSourceVoice[(int)label]->Start(0);

}

//=============================================================================
// 停止
//=============================================================================
void StopSound(SOUND_LABEL label)
{
	if (gpSourceVoice[(int)label] == NULL) return;

	XAUDIO2_VOICE_STATE xa2state;
	gpSourceVoice[(int)label]->GetState(&xa2state);
	if (xa2state.BuffersQueued)
	{
		gpSourceVoice[(int)label]->Stop(0);
	}
}

//=============================================================================
// 一時停止
//=============================================================================
void PauseSound(SOUND_LABEL label)
{
	// ごにょっとすれば可能。
}

//=============================================================================
// ユーティリティ関数群
//=============================================================================
HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;
	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;
		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}
		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}
		dwOffset += dwChunkDataSize;
		if (bytesRead >= dwRIFFDataSize) return S_FALSE;
	}
	return S_OK;
}

HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}
