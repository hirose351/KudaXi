#pragma once
#include	"component_base.h"
#include	<vector>

class DrawComponentBase : public ComponentBase
{
protected:
	int mOrderInLayer;					// 描画順
	bool mIsDraw;						// 描画するか
	std::vector<Float3>	mDrawPosList;	// 連続して描画する場合の位置リスト
	std::string mSceneName = "";		// 描画するシーン名
	Float3 mLocalPos;					// ローカル座標
	Float3 mLocalAngle;					// ローカル角度
	DirectX::XMFLOAT4X4 mLocalMtx;		// ローカル行列
	DirectX::XMFLOAT4X4 mWorldMtx;		// ワールド行列

public:
	DrawComponentBase();
	virtual ~DrawComponentBase();

	virtual void Draw() = 0;
	void ImguiDraw()override;
	virtual void ImguiDrawComponent() = 0;

	// ローカル行列作成
	void CreateLocalMtx();


	///* アクセサ *///
	void SetOrderInLayer(int _num) { mOrderInLayer = _num; };
	int GetOrderInLayer() const { return mOrderInLayer; }
	void SetDrawPos(Float3 _f3);

	void SetIsDraw(bool _flg) { mIsDraw = _flg; }
	bool GetIsDraw() { return mIsDraw; }

	void SetLocalPos(const Float3& _pos) { mLocalPos = _pos; }
	void SetLocalAngle(const Float3& _angle) { mLocalAngle = _angle; }

	const DirectX::XMFLOAT4X4 GetWorldMtx() { return mWorldMtx; }
	const Float3 GetWorldPos();
};