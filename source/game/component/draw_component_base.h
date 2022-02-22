#pragma once
#include	"component_base.h"
#include	<vector>

class DrawComponentBase : public ComponentBase
{
protected:
	int mOrderInLayer;		// 描画順
	bool mIsDraw;			// 描画するか
	std::vector<Float3>	mDrawPosList;	// 連続して描画する場合の位置リスト
	std::string mSceneName = "";

public:
	DrawComponentBase();
	virtual ~DrawComponentBase();

	virtual void Draw() = 0;

	///* アクセサ *///
	void SetOrderInLayer(int _num) { mOrderInLayer = _num; };
	int GetOrderInLayer() const { return mOrderInLayer; }
	void SetDrawPos(Float3 _f3);

	void SetIsDraw(bool _flg) { mIsDraw = _flg; }
	bool GetIsDraw() { return mIsDraw; }
};