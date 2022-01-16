#pragma once
#include	"component_base.h"
#include	<vector>

class DrawComponentBase : public ComponentBase
{
protected:
	int mOrderInLayer;		// ï`âÊèá
	std::vector<Float3>		mDrawPosList;

public:
	DrawComponentBase();
	virtual ~DrawComponentBase();

	virtual void Draw() {}

	void SetOrderInLayer(int _num) { mOrderInLayer = _num; };

	int GetOrderInLayer() const { return mOrderInLayer; }

	void SetDrawPos(Float3 _f3);
};

