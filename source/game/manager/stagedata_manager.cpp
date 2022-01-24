#include	"stagedata_manager.h"
#include	<iostream>
#include	<fstream>

using namespace std;

bool StageDataManager::SetCurrentStage(std::string _key)
{
	if (LoadStage(_key))
	{
		mCurrentStageKey = _key;
		return true;
	}
	return false;
}

const Dix::wp<StageData> StageDataManager::GetCurrentStage()
{
	if (LoadStage(mCurrentStageKey))
	{
		return GetStageData(mCurrentStageKey);
	}
	return NULL;
}

bool StageDataManager::LoadStage(std::string _key)
{
	// ���݂��邩���m���߂�
	auto it = mStageHashmap.find(_key);

	// ���݂��Ă���ΕԂ�
	if (it != mStageHashmap.end())
		return true;

	// ���݂��Ă��Ȃ���Γǂݍ���
	Dix::sp<StageData>	stagedata;

	stagedata.SetPtr(new StageData);

	string fileName = "assets/stage/" + _key + ".txt";	// �ǂݍ��ރt�@�C���̎w��

	ifstream fin(fileName, ios::in | ios::binary);
	//  �t�@�C�����J��
	//  ios::in �͓ǂݍ��ݐ�p  ios::binary �̓o�C�i���`��

	//  �t�@�C�����J���Ȃ������Ƃ��̑΍�
	if (!fin)
	{
		MessageBox(nullptr, "�X�e�[�W�t�@�C�����J���܂���", "error", MB_OK);
		return false;
	}

	char temp[128] = {};
	fin.read((char*)&temp, 128);
	stagedata->mStageName = temp;

	fin.read((char*)&stagedata->mMapSizeWidth, sizeof(int));
	fin.read((char*)&stagedata->mMapSizeHeight, sizeof(int));

	for (int z = 0; z < stagedata->mMapSizeHeight; z++)
	{
		for (int x = 0; x < stagedata->mMapSizeWidth; x++)
		{
			fin.read((char*)&stagedata->mFloorMap[z][x], sizeof(int));
			fin.read((char*)&stagedata->mMap[z][x], sizeof(int));
		}
	}
	fin.read((char*)&stagedata->mDiceCnt, sizeof(int));

	// vector�ɓ����
	for (int i = 0; i < stagedata->mDiceCnt; i++)
	{
		stagedata->mDiceMtx.emplace_back();
		fin.read((char*)&stagedata->mDiceMtx[i]._11, sizeof(float));
		fin.read((char*)&stagedata->mDiceMtx[i]._12, sizeof(float));
		fin.read((char*)&stagedata->mDiceMtx[i]._13, sizeof(float));
		fin.read((char*)&stagedata->mDiceMtx[i]._14, sizeof(float));
		fin.read((char*)&stagedata->mDiceMtx[i]._21, sizeof(float));
		fin.read((char*)&stagedata->mDiceMtx[i]._22, sizeof(float));
		fin.read((char*)&stagedata->mDiceMtx[i]._23, sizeof(float));
		fin.read((char*)&stagedata->mDiceMtx[i]._24, sizeof(float));
		fin.read((char*)&stagedata->mDiceMtx[i]._31, sizeof(float));
		fin.read((char*)&stagedata->mDiceMtx[i]._32, sizeof(float));
		fin.read((char*)&stagedata->mDiceMtx[i]._33, sizeof(float));
		fin.read((char*)&stagedata->mDiceMtx[i]._34, sizeof(float));
		fin.read((char*)&stagedata->mDiceMtx[i]._41, sizeof(float));
		fin.read((char*)&stagedata->mDiceMtx[i]._42, sizeof(float));
		fin.read((char*)&stagedata->mDiceMtx[i]._43, sizeof(float));
		fin.read((char*)&stagedata->mDiceMtx[i]._44, sizeof(float));
	}

	fin.read((char*)&stagedata->mTargetDiceType, sizeof(int));
	fin.read((char*)&stagedata->mStep, sizeof(int));

	fin.read((char*)&stagedata->mPlayerPos, sizeof(INT2));

	// unordered_map�R���e�i�Ɋi�[
	mStageHashmap[_key] = stagedata;

	fin.close();  //�t�@�C�������

	return true;
}

void StageDataManager::SaveStage(const StageData& _stagedata)
{
	ofstream fout;

	string fileName = "assets/stage/" + _stagedata.mStageName + ".txt";

	fout.open(fileName, ios::out | ios::binary | ios::trunc);
	//  �t�@�C�����J��
	//  ios::out �͏������ݐ�p�i�ȗ��j
	//  ios::binary �̓o�C�i���`���ŏo�́i�ȗ�����ƃA�X�L�[�`���ŏo�́j
	//  ios::trunc�̓t�@�C����V�K�쐬�i�ȗ��j
	//  ios::add�ɂ���ƒǋL�ɂȂ�

	//  �t�@�C�����J���Ȃ������Ƃ��̃G���[�\��
	if (!fout)
	{
		MessageBox(nullptr, "�X�e�[�W�t�@�C�����J���܂���", "error", MB_OK);
		return;
	}

	char temp[128] = {};
	_stagedata.mStageName.copy(temp, 128);

	fout.write((char*)&temp, 128);

	fout.write((char*)&_stagedata.mMapSizeWidth, sizeof(int));
	fout.write((char*)&_stagedata.mMapSizeHeight, sizeof(int));

	for (int z = 0; z < _stagedata.mMapSizeHeight; z++)
	{
		for (int x = 0; x < _stagedata.mMapSizeWidth; x++)
		{
			fout.write((char*)&_stagedata.mFloorMap[z][x], sizeof(int));
			fout.write((char*)&_stagedata.mMap[z][x], sizeof(int));
		}
	}

	int diceCnt = static_cast<int>(_stagedata.mDiceMtx.size());
	fout.write((char*)&diceCnt, sizeof(int));

	for (auto d : _stagedata.mDiceMtx)
	{
		fout.write((char*)&d._11, sizeof(float));
		fout.write((char*)&d._12, sizeof(float));
		fout.write((char*)&d._13, sizeof(float));
		fout.write((char*)&d._14, sizeof(float));
		fout.write((char*)&d._21, sizeof(float));
		fout.write((char*)&d._22, sizeof(float));
		fout.write((char*)&d._23, sizeof(float));
		fout.write((char*)&d._24, sizeof(float));
		fout.write((char*)&d._31, sizeof(float));
		fout.write((char*)&d._32, sizeof(float));
		fout.write((char*)&d._33, sizeof(float));
		fout.write((char*)&d._34, sizeof(float));
		fout.write((char*)&d._41, sizeof(float));
		fout.write((char*)&d._42, sizeof(float));
		fout.write((char*)&d._43, sizeof(float));
		fout.write((char*)&d._44, sizeof(float));
	}

	fout.write((char*)&_stagedata.mTargetDiceType, sizeof(int));
	fout.write((char*)&_stagedata.mStep, sizeof(int));
	fout.write((char*)&_stagedata.mPlayerPos, sizeof(INT2));

	fout.close();  //�t�@�C�������
}

const Dix::wp<StageData> StageDataManager::GetStageData(string _key)
{
	// ���݂��邩�m���߂�
	auto it = mStageHashmap.find(_key);
	if (it == mStageHashmap.end())
	{
		MessageBox(nullptr, "�w�肳�ꂽ�f�[�^�͑��݂��܂���", "error", MB_OK);
		return NULL;
	}

	return mStageHashmap[_key];
}

void StageDataManager::RemoveStageData(std::string _key)
{
	// ���݂��邩���m���߂�
	auto it = mStageHashmap.find(_key);
	if (it == mStageHashmap.end())
	{
		MessageBox(nullptr, "�w�肳�ꂽ�f�[�^�͑��݂��܂���", "error", MB_OK);
		return;
	}
	// txt�t�@�C�����폜
	string fileName = "assets/stage/" + _key + ".txt";
	remove(fileName.c_str());
	// �}�b�v����X�e�[�W�f�[�^���폜
	mStageHashmap.erase(_key);
}