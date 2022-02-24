#pragma once
#include	"component_base.h"
#include	<vector>

class DrawComponentBase : public ComponentBase
{
protected:
	int mOrderInLayer;					// �`�揇
	bool mIsDraw;						// �`�悷�邩
	std::vector<Float3>	mDrawPosList;	// �A�����ĕ`�悷��ꍇ�̈ʒu���X�g
	std::string mSceneName = "";		// �`�悷��V�[����
	Float3 mLocalPos;					// ���[�J�����W
	Float3 mLocalAngle;					// ���[�J���p�x
	DirectX::XMFLOAT4X4 mLocalMtx;		// ���[�J���s��
	DirectX::XMFLOAT4X4 mWorldMtx;		// ���[���h�s��

public:
	DrawComponentBase();
	virtual ~DrawComponentBase();

	virtual void Draw() = 0;
	void ImguiDraw()override;
	virtual void ImguiDrawComponent() = 0;

	// ���[�J���s��쐬
	void CreateLocalMtx();


	///* �A�N�Z�T *///
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