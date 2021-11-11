#include	"game.h"
#include	"Application.h"
#include	"game/manager/SceneManager.h"
#include	"game/scene/MaingameScene.h"
#include	"game/component/CCamera.h"
#include	"system/dx11/CDirectInput.h"
#include	"system/dx11/DX11Settransform.h"
#include	"system/imgui/util/myimgui.h"
#include	"system/util/2dsystem.h"
#include	"system/util/XAudio2.h"


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

	CCamera::GetInstance()->Init(
		10.0f,							// �j�A�N���b�v
		10000.0f,						// �t�@�[�N���b�v
		XM_PI / 5.0f,					// ����p
		static_cast<float>(Application::CLIENT_WIDTH),		// �X�N���[����
		static_cast<float>(Application::CLIENT_HEIGHT),		// �X�N���[���̍���
		eye, lookat, up);				// �J�����̃f�[�^

	// ���s�������Z�b�g
	DX11LightInit(
		DirectX::XMFLOAT4(1, 1, -1, 0));

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
	SceneManager::GetInstance()->SetFadeColor(XMFLOAT4(0.9f, 0.8f, 0.8f, 1));

	// �V�[���}�l�[�W���Ƀ^�C�g���V�[����o�^
	SceneManager::GetInstance()->add<MaingameScene>("GameMain");
	//SceneManager::GetInstance()->add<StageCreateScene>("Create");
	//SceneManager::GetInstance()->add<ModeSelectScene>("Mode");
	//SceneManager::GetInstance()->add<TitleScene>("Title");
	// ���݃V�[����Title�ɂ���
	SceneManager::GetInstance()->setCurrentScene("GameMain");
}

void GameInput(uint64_t dt) {
	CDirectInput::GetInstance().GetKeyBuffer();
	CDirectInput::GetInstance().GetMouseState();
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
	mtx = CCamera::GetInstance()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	// �r���[�ϊ��s����擾
	mtx = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	SceneManager::GetInstance()->Render();

	// �`��㏈��
	DX11AfterRender();
}


#include "game/gameobject/Actor.h"
void GameDispose() {
	UninitSound();
	imguiExit();
	DX11Uninit();
}


Game::Game()
	:mIsRunning(true)
	, mUpdatingActors(false)
{

}

bool Game::Initialize()
{
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

	CCamera::GetInstance()->Init(
		10.0f,							// �j�A�N���b�v
		10000.0f,						// �t�@�[�N���b�v
		XM_PI / 5.0f,					// ����p
		static_cast<float>(Application::CLIENT_WIDTH),		// �X�N���[����
		static_cast<float>(Application::CLIENT_HEIGHT),		// �X�N���[���̍���
		eye, lookat, up);				// �J�����̃f�[�^

	// ���s�������Z�b�g
	DX11LightInit(
		DirectX::XMFLOAT4(1, 1, -1, 0));

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

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	//SDL_Event event;
	//while (SDL_PollEvent(&event))
	//{
	//	switch (event.type)
	//	{
	//	case SDL_QUIT:
	//		mIsRunning = false;
	//		break;
	//	}
	//}

	//const Uint8* state = SDL_GetKeyboardState(NULL);
	//if (state[SDL_SCANCODE_ESCAPE])
	//{
	//	mIsRunning = false;
	//}

	//// Process ship input
	//mShip->ProcessKeyboard(state);
}

void Game::UpdateGame()
{
	// �f���^�^�C�����v�Z
	//// Wait until 16ms has elapsed since last frame
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
	//	;

	float deltaTime = /*(SDL_GetTicks() - mTicksCount) /*/ 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	//mTicksCount = SDL_GetTicks();

	// ���ׂẴA�N�^�[���X�V
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// �҂��ɂȂ��Ă����A�N�^�[��m_actors�Ɉړ�
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// ���񂾃A�N�^�[���ꎞ�z��ɒǉ�
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// ���񂾃A�N�^�[�������im_actors����폜�j
	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	//SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	//SDL_RenderClear(mRenderer);

	//// Draw all sprite components
	//for (auto sprite : mSprites)
	//{
	//	sprite->Draw(mRenderer);
	//}

	//SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
	//// Create player's ship
	//mShip = new Ship(this);
	//mShip->SetPosition(Vector2(100.0f, 384.0f));
	//mShip->SetScale(1.5f);

	//// Create actor for the background (this doesn't need a subclass)
	//Actor* temp = new Actor(this);
	//temp->SetPosition(Vector2(512.0f, 384.0f));
	//// Create the "far back" background
	//BGSpriteComponent* bg = new BGSpriteComponent(temp);
	//bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	//std::vector<SDL_Texture*> bgtexs = {
	//	GetTexture("Assets/Farback01.png"),
	//	GetTexture("Assets/Farback02.png")
	//};
	//bg->SetBGTextures(bgtexs);
	//bg->SetScrollSpeed(-100.0f);
	//// Create the closer background
	//bg = new BGSpriteComponent(temp, 50);
	//bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	//bgtexs = {
	//	GetTexture("Assets/Stars.png"),
	//	GetTexture("Assets/Stars.png")
	//};
	//bg->SetBGTextures(bgtexs);
	//bg->SetScrollSpeed(-200.0f);
}

void Game::UnloadData()
{
	// �A�N�^�[���폜
	// ~Actor��RemoveActor���Ăяo�����̂ŁA�ʂ̎�ނ̃��[�v���g��
	while (!mActors.empty())	// �R���e�i����Ȃ�
	{
		delete mActors.back();
	}

	// Destroy textures
	//for (auto i : mTextures)
	//{
	//	SDL_DestroyTexture(i.second);
	//}
	//mTextures.clear();
}
//
//SDL_Texture* Game::GetTexture(const std::string& fileName)
//{
//	SDL_Texture* tex = nullptr;
//	// Is the texture already in the map?
//	auto iter = mTextures.find(fileName);
//	if (iter != mTextures.end())
//	{
//		tex = iter->second;
//	}
//	else
//	{
//		// Load from file
//		SDL_Surface* surf = IMG_Load(fileName.c_str());
//		if (!surf)
//		{
//			SDL_Log("Failed to load texture file %s", fileName.c_str());
//			return nullptr;
//		}
//
//		// Create texture from surface
//		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
//		SDL_FreeSurface(surf);
//		if (!tex)
//		{
//			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
//			return nullptr;
//		}
//
//		mTextures.emplace(fileName.c_str(), tex);
//	}
//	return tex;
//}

void Game::Shutdown()
{
	UnloadData();
	//IMG_Quit();
	//SDL_DestroyRenderer(mRenderer);
	//SDL_DestroyWindow(mWindow);
	//SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	// �A�N�^�[���X�V���Ȃ�҂��Q�ɒǉ�
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// �ۗ����̃A�N�^�[���ǂ���
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// �x�N�g���̍Ō�܂Ń��b�v���ă|�b�v�I�t���܂��i�R�s�[�̏����͔����Ă��������j
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// ���݂���A�N�^�[���ǂ���
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// �x�N�g���̍Ō�܂Ń��b�v���ă|�b�v�I�t���܂��i�R�s�[�̏����͔����Ă��������j
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

//void Game::AddSprite(SpriteComponent* sprite)
//{
//	// Find the insertion point in the sorted vector
//	// (The first element with a higher draw order than me)
//	int myDrawOrder = sprite->GetDrawOrder();
//	auto iter = mSprites.begin();
//	for (;
//		 iter != mSprites.end();
//		 ++iter)
//	{
//		if (myDrawOrder < (*iter)->GetDrawOrder())
//		{
//			break;
//		}
//	}
//
//	// Inserts element before position of iterator
//	mSprites.insert(iter, sprite);
//}
//
//void Game::RemoveSprite(SpriteComponent* sprite)
//{
//	// (We can't swap because it ruins ordering)
//	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
//	mSprites.erase(iter);
//}