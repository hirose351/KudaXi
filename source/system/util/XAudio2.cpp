//=============================================================================
//
// �T�E���h���� [XAudio2.cpp]
//
//=============================================================================

#include	"XAudio2.h"

// �p�����[�^�\����
typedef struct
{
	LPCSTR filename;	// �����t�@�C���܂ł̃p�X��ݒ�
	bool bLoop;			// true�Ń��[�v�B�ʏ�BGM��ture�ASE��false�B
} PARAM;

PARAM gParam[SOUND_LABEL_MAX] =
{
	// BGM
	{"assets/audio/bgm/title.wav", true},		// �^�C�g��BGM�i���[�v������̂�true�ݒ�j
	{"assets/audio/bgm/game.wav", true},		// �Q�[��BGM
	{"assets/audio/bgm/create.wav", true},		// �X�e�[�W�N���G�C�gBGM
	// SE
	{"assets/audio/se/switch.wav", false},  	// �؂�ւ�SE�i���[�v���Ȃ��̂�false�ݒ�j
	{"assets/audio/se/ok.wav", false},			// ����SE
	{"assets/audio/se/dice.wav", false},		// �T�C�R��SE

	{"assets/audio/se/miss.wav", false},		// �~�XSE
	{"assets/audio/se/yeah.wav", false},		// �N���ASE
	{"assets/audio/se/start.wav", false},		// �X�^�[�gSE
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
//    �O���[�o���ϐ�
//-----------------------------------------------------------------
IXAudio2               *gpXAudio2 = NULL;
IXAudio2MasteringVoice *gpMasteringVoice = NULL;
IXAudio2SourceVoice    *gpSourceVoice[SOUND_LABEL_MAX];

WAVEFORMATEXTENSIBLE	gWfx[SOUND_LABEL_MAX];			// WAV�t�H�[�}�b�g
XAUDIO2_BUFFER			gBuffer[SOUND_LABEL_MAX];
BYTE					*gDataBuffer[SOUND_LABEL_MAX];

//-----------------------------------------------------------------
//    �v���g�^�C�v�錾
//-----------------------------------------------------------------
HRESULT FindChunk(HANDLE, DWORD, DWORD&, DWORD&);
HRESULT ReadChunkData(HANDLE, void*, DWORD, DWORD);

//=============================================================================
// ������
//=============================================================================
HRESULT InitSound()
{
	HRESULT      hr;

	HANDLE               hFile;
	DWORD                dwChunkSize;
	DWORD                dwChunkPosition;
	DWORD                filetype;

	// �b�n�l�̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	/**** Create XAudio2 ****/
	hr = XAudio2Create(&gpXAudio2, 0);		// �������ͤ����t���O �f�o�b�O���[�h�̎w��(���݂͖��g�p�Ȃ̂�0�ɂ���)
	//hr=XAudio2Create(&g_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);		// ��O�����́Awindows�ł͖���
	if (FAILED(hr))
	{
		CoUninitialize();
		return -1;
	}

	/**** Create Mastering Voice ****/
	hr = gpXAudio2->CreateMasteringVoice(&gpMasteringVoice);			// ����͂o�b�̃f�t�H���g�ݒ�ɔC���Ă���
	/*, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL*/		// �{���U�̈����������Ă���
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

		// 	�T�u�~�b�g�{�C�X�ŗ��p����T�u�~�b�g�o�b�t�@�̐ݒ�
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
// �J������
//=============================================================================
void UninitSound(void)
{
	for (int i = 0; i < SOUND_LABEL_MAX; i++)
	{
		if (gpSourceVoice[i])
		{
			gpSourceVoice[i]->Stop(0);
			gpSourceVoice[i]->FlushSourceBuffers();
			gpSourceVoice[i]->DestroyVoice();			// �I�[�f�B�I�O���t����\�[�X�{�C�X���폜
			delete[]  gDataBuffer[i];
		}
	}

	gpMasteringVoice->DestroyVoice();

	if (gpXAudio2) gpXAudio2->Release();

	// �b�n�l�̔j��
	CoUninitialize();
}

//=============================================================================
// �Đ�
//=============================================================================
void PlaySound(SOUND_LABEL label)
{
	// �\�[�X�{�C�X�쐬
	//g_pXAudio2->CreateSourceVoice(&(g_pSourceVoice[(int)label]), &(g_wfx[(int)label].Format));
	gpSourceVoice[(int)label]->SubmitSourceBuffer(&(gBuffer[(int)label]));	// �{�C�X�L���[�ɐV�����I�[�f�B�I�o�b�t�@�[��ǉ�

	// �Đ�
	gpSourceVoice[(int)label]->Start(0);

}

//=============================================================================
// ��~
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
// �ꎞ��~
//=============================================================================
void PauseSound(SOUND_LABEL label)
{
	// ���ɂ���Ƃ���Ή\�B
}

//=============================================================================
// ���[�e�B���e�B�֐��Q
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
