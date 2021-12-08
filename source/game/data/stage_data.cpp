#include "stage_data.h"

void StageData::SetStageData(const StageData * _stageData)
{
	mStageName = _stageData->mStageName;
	mMapSizeWidth = _stageData->mMapSizeWidth;
	mMapSizeHeight = _stageData->mMapSizeHeight;

	memcpy(mFloorMap, _stageData->mFloorMap, sizeof(mFloorMap));
	memcpy(mMap, _stageData->mMap, sizeof(mFloorMap));

	//for (int z = 0; z < 10; z++)
	//{
	//	for (int x = 0; x < 10; x++)
	//	{
	//		// ”z—ñ‚É“ü‚ê‚é
	//		mFloorMap[z][x] = _stageData->mFloorMap[z][x];
	//		mMap[z][x] = _stageData->mMap[z][x];
	//	}
	//}

	mDiceCnt = _stageData->mDiceCnt;
	mDiceMtx = _stageData->mDiceMtx;
	mDiceAlignCnt = _stageData->mDiceAlignCnt;
	mTargetDiceType = _stageData->mTargetDiceType;
	mStep = _stageData->mStep;
	mPlayerPos = _stageData->mPlayerPos;
	mMapChipSize = _stageData->mMapChipSize;
}