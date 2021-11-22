#include	"Player.h"
#include	<DirectXMath.h>
#include	<string>
#include	"../../system/dx11/CDirectInput.h"
#include	"../../system/dx11/DX11Settransform.h"
#include	"../../system/model/ModelMgr.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_MOVE_MODEL	(0.085f)				// 移動速度
#define	VALUE_ROTATE_MODEL	(XM_PI * 0.025f)		// 回転速度
#define	RATE_ROTATE_MODEL	(0.10f)					// 回転慣性係数
#define	RATE_MOVE_MODEL		(0.1f)
//#define	RATE_MOVE_MODEL		(0.025f)			// 移動慣性係数

Player::Player()
{
	bool sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/player/player.fbx",
		"shader/vs.hlsl",
		"shader/ps.hlsl",
		"assets/model/player/");
	//単位行列化
	DX11MtxIdentity(mMtx);
	// 初期姿勢
	mRotation = { 0.0f, 0.0f, 0.0f };
	// 目標姿勢
	mDestrot = { 0.0f, 0.0f, 0.0f };
	// 移動量
	mMove = { 0.0f, 0.0f, 0.0f };

	SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/player/player.fbx"));

	Init();
}

Player::~Player()
{
	Finalize();
};

void Player::Init()
{
	// 初期姿勢
	mRotation = { 0.0f, 0.0f, 0.0f };
	// 目標姿勢
	mDestrot = { 0.0f, 0.0f, 0.0f };
	// 移動量
	mMove = { 0.0f, 0.0f, 0.0f };

	mDirection = DIRECTION::DOWN;
	mIsDiceMove = false;
}

void Player::Update()
{
	Vector3 rotCamera;
	float radian;
	if (!mIsDiceMove)
	{
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_A))
		{
			// 左移動
			radian = rotCamera.y + XM_PI * 0.50f;

			mMove.x -= sinf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
			mMove.z -= cosf(XM_PI * 0.50f) * VALUE_MOVE_MODEL;
			//// z軸調整
			//if (m_pos.z > (-m_mapPos.z*DICESCALE)) {
			//	m_move.z -= 0.01f;
			//}
			//else {
			//	m_move.z += 0.01f;
			//}
			// 目標角度をセット
			mDestrot.y = radian;
			mDirection = DIRECTION::LEFT;
		}
		else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_D))
		{
			// 右移動
			radian = rotCamera.y - XM_PI * 0.50f;

			mMove.x -= sinf(radian) * VALUE_MOVE_MODEL;
			mMove.z -= cosf(radian) * VALUE_MOVE_MODEL;
			//// z軸調整
			//if (m_pos.z > (-m_mapPos.z*DICESCALE)) {
			//	m_move.z -= 0.01f;
			//}
			//else {
			//	m_move.z += 0.01f;
			//}
			// 目標角度をセット
			mDestrot.y = radian;
			mDirection = DIRECTION::RIGHT;
		}
		else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_W))
		{
			// 前移動
			mMove.x -= sinf(XM_PI) * VALUE_MOVE_MODEL;
			mMove.z -= cosf(XM_PI) * VALUE_MOVE_MODEL;
			//// x軸調整
			//if (m_pos.x > m_mapPos.x*DICESCALE) {
			//	m_move.x -= 0.01f;
			//}
			//else {
			//	m_move.x += 0.01f;
			//}
			// 目標角度をセット
			mDestrot.y = rotCamera.y + XM_PI;
			mDirection = DIRECTION::UP;
		}
		else if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_S))
		{
			// 後移動
			mMove.x -= sinf(rotCamera.y) * VALUE_MOVE_MODEL;
			mMove.z -= cosf(rotCamera.y) * VALUE_MOVE_MODEL;
			//// x軸調整
			//if (m_pos.x > m_mapPos.x*DICESCALE) {
			//	m_move.x -= 0.01f;
			//}
			//else {
			//	m_move.x += 0.01f;
			//}
			// 目標角度をセット
			mDestrot.y = rotCamera.y;
			mDirection = DIRECTION::DOWN;
		}

		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RETURN))
		{
			Init();
		}
	}
	// 目標角度と現在角度との差分を求める
	float diffrot = mDestrot.y - mRotation.y;
	if (diffrot > XM_PI)
	{
		diffrot -= XM_PI * 2.0f;
	}
	if (diffrot < -XM_PI)
	{
		diffrot += XM_PI * 2.0f;
	}

	// 角度速度に慣性を付ける
	mRotation.y += diffrot * RATE_ROTATE_MODEL;
	if (mRotation.y > XM_PI)
	{
		mRotation.y -= XM_PI * 2.0f;
	}
	if (mRotation.y < -XM_PI)
	{
		mRotation.y += XM_PI * 2.0f;
	}

	/// 位置移動
	mPosition += mMove;

	// 移動量に慣性をかける(減速慣性)
	mMove += (mMove*-1.0f) * RATE_MOVE_MODEL;

	// 回転を反映、平行移動を反映
	Vector3 degree = (mRotation* 180.0f) / XM_PI;
	DX11MakeWorldMatrix(mMtx, degree, mPosition);
	DirectX::XMFLOAT4X4 scaleMtx;
	DX11MtxScale(1.3f, 1.3f, 1.3f, scaleMtx);
	DX11MtxMultiply(mMtx, scaleMtx, mMtx);
}
void Player::Draw()
{
	mpModel->Draw(mMtx);
}

void Player::Finalize()
{
	//mpModel->Uninit();
}