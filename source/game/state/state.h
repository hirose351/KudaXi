#pragma once
#include	"../gameobject/transform.h"
#include	"../../system/util/dixsmartptr.h"


enum Pstate {
	eStop,
	eMove,
	ePush,
	eRoll,
};

class State
{
	Dix::wp<Transform> mTramsform;

	//DIRECTION mDiceMoveDirection;				// サイコロを回転させる方向
	//bool mIsDiceMove = false;					// サイコロが回転しているか
	//DIRECTION mDirection;						// プレイヤーの方向(キー参照)
	//Float3					mDestrot;			// 目標姿勢
	//DIRECTION				mMoveKeySts;		// 押されている移動キー

};