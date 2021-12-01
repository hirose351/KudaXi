#include	"stage.h"

void Stage::ObjectInit()
{
	//for (int cnt = 0; cnt<;i++;)
	//{
	//	// À•WŽw’è
	//	(*plane)->GetTransform()->SetPosition(mCurrentStageData.mFloorMap[cnt%mCurrentStageData.mMapSizeWidth][cnt]);

	//	plane++;
	//}
	mPlane.Init();
}

void Stage::ObjectDraw()
{
	//for (auto plane : planeList)
	//{
	//	(*plane).Draw();
	//}
	for (int z = 0; z < mCurrentStageData.mMapSizeHeight; z++)
	{
		for (int x = 0; x < mCurrentStageData.mMapSizeWidth; x++)
		{
			if (mCurrentStageData.mFloorMap[x][z] != 0)
			{
				mPlane.GetTransform()->SetPosition(Float3(DICESCALE*x, 0, -DICESCALE * z));
				mPlane.Draw();
			}
		}
	}
}
