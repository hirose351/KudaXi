#pragma once
#include		"../../system/util/vector.h"
#include		"../../system/util/dixsmartptr.h"
#include		<vector>
#define		STAGESIZEMAX			(10)


class StageData
{
public:
	StageData() {
		mStageName = "Test";
	}

	~StageData() { Finalize(); }

	// ��n��
	void Finalize()
	{
		//if (!mDiceMtx.empty())
		//{
		//	mDiceMtx.clear();
		//	mDiceMtx.shrink_to_fit();
		//}
	}

	std::string mStageName;

	int mMapSizeWidth = 8;			// �}�b�v���T�C�Y
	int mMapSizeHeight = 8;		// �}�b�v�c�T�C�Y

	// ���z�u�p�}�b�v�z��
	int mFloorMap[STAGESIZEMAX][STAGESIZEMAX] = {
		// 0:�Ȃ� 1:���A�� 
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
			1,1,0,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,
	};

	// �u���b�N�����p�}�b�v�z��
	int mMap[10][10] = {
		// 0:�Ȃ� 1:Dice�z�u
		// 0:�Ȃ� 1:Dice�z�u
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1, 1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1, 1, 1,-1, 1,-1,-1,-1,-1,-1,
			-1, 1,-1,-1, 1,-1,-1,-1,-1,-1,
			-1, 1,-1,-1, 1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
			-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	};

	int mDiceCnt;									// Dice�̑��� 
	std::vector<DirectX::XMFLOAT4X4> mDiceMtx;		// �e�T�C�R���̍s��

	int mDiceAlignCnt = 3;							// ����������ɂȂ鐔
	int mTargetDiceType = 3;						// ������T�C�R���̖�
	int mStep = 5;									// �T�C�R���̈ړ��\��

	Float3 mPlayerPos;								// �v���C���[�̏����ʒu

	float mMapChipSize = 15.0f;

	int GetFloorMap(int _cnt)
	{
		return mFloorMap[_cnt%mMapSizeWidth][_cnt / mMapSizeWidth];
	}
};

