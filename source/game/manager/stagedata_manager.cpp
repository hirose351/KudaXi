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
	// 存在するかを確かめる
	auto it = mStageHashmap.find(_key);

	// 存在していれば返す
	if (it != mStageHashmap.end())
		return true;

	// 存在していなければ読み込む
	Dix::sp<StageData>	stagedata;

	stagedata.SetPtr(new StageData);

	string fileName = "assets/stage/" + _key + ".txt";	// 読み込むファイルの指定

	ifstream fin(fileName, ios::in | ios::binary);
	//  ファイルを開く
	//  ios::in は読み込み専用  ios::binary はバイナリ形式

	//  ファイルが開けなかったときの対策
	if (!fin)
	{
		MessageBox(nullptr, "ステージファイルが開けません", "error", MB_OK);
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

	// vectorに入れる
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

	// unordered_mapコンテナに格納
	mStageHashmap[_key] = stagedata;

	fin.close();  //ファイルを閉じる

	return true;
}

void StageDataManager::SaveStage(const StageData& _stagedata)
{
	ofstream fout;

	string fileName = "assets/stage/" + _stagedata.mStageName + ".txt";

	fout.open(fileName, ios::out | ios::binary | ios::trunc);
	//  ファイルを開く
	//  ios::out は書き込み専用（省略可）
	//  ios::binary はバイナリ形式で出力（省略するとアスキー形式で出力）
	//  ios::truncはファイルを新規作成（省略可）
	//  ios::addにすると追記になる

	//  ファイルが開けなかったときのエラー表示
	if (!fout)
	{
		MessageBox(nullptr, "ステージファイルが開けません", "error", MB_OK);
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

	fout.close();  //ファイルを閉じる
}

const Dix::wp<StageData> StageDataManager::GetStageData(string _key)
{
	// 存在するか確かめる
	auto it = mStageHashmap.find(_key);
	if (it == mStageHashmap.end())
	{
		MessageBox(nullptr, "指定されたデータは存在しません", "error", MB_OK);
		return NULL;
	}

	return mStageHashmap[_key];
}

void StageDataManager::RemoveStageData(std::string _key)
{
	// 存在するかを確かめる
	auto it = mStageHashmap.find(_key);
	if (it == mStageHashmap.end())
	{
		MessageBox(nullptr, "指定されたデータは存在しません", "error", MB_OK);
		return;
	}
	// txtファイルを削除
	string fileName = "assets/stage/" + _key + ".txt";
	remove(fileName.c_str());
	// マップからステージデータを削除
	mStageHashmap.erase(_key);
}