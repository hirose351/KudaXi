#include	"game.h"
#include	"application.h"
#include	"game/manager/scene_manager.h"
#include	"game/scene/title_scene.h"
#include	"game/scene/maingame_scene.h"
#include	"game/scene/modeselect_scene.h"
#include	"game/scene/stagecreate_scene.h"
#include	"game/gameobject/camera.h"
#include	"system/dx11/CDirectInput.h"
#include	"system/dx11/DX11Settransform.h"
#include	"system/imgui/util/myimgui.h"
#include	"system/util/2dsystem.h"
#include	"system/util/XAudio2.h"
#include	"system/util/controller_input.h"

void GameInit() {

	// DX11������
	DX11Init(
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		false);

	// �J����
	DirectX::XMFLOAT3 eye(140, 130, -170);	//�J�����̈ʒu
	DirectX::XMFLOAT3 lookat(0, 0, 0);	//�����_
	DirectX::XMFLOAT3 up(0, 1, 0);		//�J�����̏�����x�N�g��

	Camera::GetInstance()->Init(
		10.0f,							// �j�A�N���b�v
		10000.0f,						// �t�@�[�N���b�v
		XM_PI / 5.0f,					// ����p
		static_cast<float>(Application::CLIENT_WIDTH),		// �X�N���[����
		static_cast<float>(Application::CLIENT_HEIGHT),		// �X�N���[���̍���
		eye, lookat, up);				// �J�����̃f�[�^

	// ���s�������Z�b�g
	DX11LightInit(
		DirectX::XMFLOAT4(-50, -87, 66, 0));

	// �A���t�@�u�����h�L����
	TurnOnAlphablend();

	// 2D�V�X�e��������
	Init2D();

	// DirectT INPUT ������
	CDirectInput::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT);

	// IMGUI������
	imguiInit();

	// 
	InitSound();

	GameSceneInit();
}

void GameSceneInit()
{
	// �V�[���}�l�[�W���Ƀ^�C�g���V�[����o�^
	SceneManager::GetInstance()->AddScene<MaingameScene>("GameMain");
	SceneManager::GetInstance()->AddScene<StageCreateScene>("Create");
	SceneManager::GetInstance()->AddScene<ModeSelectScene>("Mode");
	SceneManager::GetInstance()->AddScene<TitleScene>("Title");
	// ���݃V�[����Title�ɂ���
	SceneManager::GetInstance()->SetCurrentScene("Title");
}

void GameInput(uint64_t dt) {
	CDirectInput::GetInstance().GetKeyBuffer();
	CDirectInput::GetInstance().GetMouseState();
	ControllerInput::GetInstance().Input();
}

void UpdateCamera()
{

}

void GameUpdate(uint64_t dt) {
	SceneManager::GetInstance()->Update();
}

void GameRender(uint64_t dt) {

	float col[4] = { 0,0,1,1 };	// �h��Ԃ��J���[

	// �`��O����
	DX11BeforeRender(col);

	XMFLOAT4X4 mtx;

	// �v���W�F�N�V�����ϊ��s��擾
	mtx = Camera::GetInstance()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eProjection, mtx);

	// �r���[�ϊ��s����擾
	mtx = Camera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::eView, mtx);

	SceneManager::GetInstance()->Render();

	// �`��㏈��
	DX11AfterRender();
}

void GameDispose() {
	UninitSound();
	imguiExit();
	DX11Uninit();
}