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

	// DX11初期化
	DX11Init(
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		false);

	// カメラ
	DirectX::XMFLOAT3 eye(140, 130, -170);	//カメラの位置
	DirectX::XMFLOAT3 lookat(0, 0, 0);	//注視点
	DirectX::XMFLOAT3 up(0, 1, 0);		//カメラの上向きベクトル

	CCamera::GetInstance()->Init(
		10.0f,							// ニアクリップ
		10000.0f,						// ファークリップ
		XM_PI / 5.0f,					// 視野角
		static_cast<float>(Application::CLIENT_WIDTH),		// スクリーン幅
		static_cast<float>(Application::CLIENT_HEIGHT),		// スクリーンの高さ
		eye, lookat, up);				// カメラのデータ

	// 平行光源をセット
	DX11LightInit(
		DirectX::XMFLOAT4(1, 1, -1, 0));

	// アルファブレンド有効化
	TurnOnAlphablend();

	// 2Dシステム初期化
	Init2D();

	// DirectT INPUT 初期化
	CDirectInput::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT);

	// IMGUI初期化
	imguiInit();

	// 
	InitSound();

	GameSceneInit();
}

void GameSceneInit()
{
	SceneManager::GetInstance()->SetFadeColor(XMFLOAT4(0.9f, 0.8f, 0.8f, 1));

	// シーンマネージャにタイトルシーンを登録
	SceneManager::GetInstance()->add<MaingameScene>("GameMain");
	//SceneManager::GetInstance()->add<StageCreateScene>("Create");
	//SceneManager::GetInstance()->add<ModeSelectScene>("Mode");
	//SceneManager::GetInstance()->add<TitleScene>("Title");
	// 現在シーンをTitleにする
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

	float col[4] = { 0,0,1,1 };	// 塗りつぶしカラー

	// 描画前処理
	DX11BeforeRender(col);

	XMFLOAT4X4 mtx;

	// プロジェクション変換行列取得
	mtx = CCamera::GetInstance()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	// ビュー変換行列を取得
	mtx = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	SceneManager::GetInstance()->Render();

	// 描画後処理
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
	// DX11初期化
	DX11Init(
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		false);

	// カメラ
	DirectX::XMFLOAT3 eye(140, 130, -170);	//カメラの位置
	DirectX::XMFLOAT3 lookat(0, 0, 0);	//注視点
	DirectX::XMFLOAT3 up(0, 1, 0);		//カメラの上向きベクトル

	CCamera::GetInstance()->Init(
		10.0f,							// ニアクリップ
		10000.0f,						// ファークリップ
		XM_PI / 5.0f,					// 視野角
		static_cast<float>(Application::CLIENT_WIDTH),		// スクリーン幅
		static_cast<float>(Application::CLIENT_HEIGHT),		// スクリーンの高さ
		eye, lookat, up);				// カメラのデータ

	// 平行光源をセット
	DX11LightInit(
		DirectX::XMFLOAT4(1, 1, -1, 0));

	// アルファブレンド有効化
	TurnOnAlphablend();

	// 2Dシステム初期化
	Init2D();

	// DirectT INPUT 初期化
	CDirectInput::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT);

	// IMGUI初期化
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
	// デルタタイムを計算
	//// Wait until 16ms has elapsed since last frame
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
	//	;

	float deltaTime = /*(SDL_GetTicks() - mTicksCount) /*/ 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	//mTicksCount = SDL_GetTicks();

	// すべてのアクターを更新
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// 待ちになっていたアクターをm_actorsに移動
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// 死んだアクターを一時配列に追加
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// 死んだアクターを消す（m_actorsから削除）
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
	// アクターを削除
	// ~ActorでRemoveActorが呼び出されるので、別の種類のループを使う
	while (!mActors.empty())	// コンテナが空なら
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
	// アクターが更新中なら待ち群に追加
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
	// 保留中のアクターかどうか
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// ベクトルの最後までワップしてポップオフします（コピーの消去は避けてください）
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// 存在するアクターかどうか
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// ベクトルの最後までワップしてポップオフします（コピーの消去は避けてください）
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