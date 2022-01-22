#pragma once
#include	"component_base.h"
#include	<vector>

class DrawComponentBase : public ComponentBase
{
protected:
	int mOrderInLayer;		// 描画順
	std::vector<Float3>		mDrawPosList;
	bool mIsDraw;			// 描画するか

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