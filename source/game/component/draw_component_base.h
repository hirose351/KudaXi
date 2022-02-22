#pragma once
#include	"component_base.h"
#include	<vector>

class DrawComponentBase : public ComponentBase
{
protected:
	int mOrderInLayer;		// �`�揇
	bool mIsDraw;			// �`�悷�邩
	std::vector<Float3>	mDrawPosList;	// �A�����ĕ`�悷��ꍇ�̈ʒu���X�g
	std::string mSceneName = "";

public:
	DrawComponentBase();
	virtual ~DrawComponentBase();

	virtual void Draw() = 0;

	///* �A�N�Z�T *///
	void SetOrderInLayer(int _num) { mOrderInLayer = _num; };
	int GetOrderInLayer() const { return mOrderInLayer; }
	void SetDrawPos(Float3 _f3);

	void SetIsDraw(bool _flg) { mIsDraw = _flg; }
	bool GetIsDraw() { return mIsDraw; }
};