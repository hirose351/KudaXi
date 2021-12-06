#include	"scene_base.h"
#include	"../gameobject/gameobject.h"
#include	"../../system/imgui/util/myimgui.h"

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
	mObjectList.clear();
	mObjectList.shrink_to_fit();
}

void SceneBase::AddGameObject(GameObject* _object)
{
	mObjectList.emplace_back(_object);
}

bool SceneBase::Init()
{
	for (auto &obj : mObjectList)
	{
		obj->Init();
	}
	return true;
}

void SceneBase::Update()
{
	for (auto &obj : mObjectList)
	{
		obj->Update();
	}
	SceneUpdate();
}

void SceneBase::Render()
{
	for (auto &obj : mObjectList)
	{
		obj->Draw();
	}
	SceneRender();
	imguiDraw(std::bind(&SceneBase::ImguiDebug, std::ref(*this)));
}

void SceneBase::ImguiDebug()
{
	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(280, 300), ImGuiCond_Once);
	ImGui::Begin(u8"GameObject");
	for (auto &obj : mObjectList)
	{
		obj->ImguiDraw();
	}
	ImGui::End();

}

bool SceneBase::Dispose()
{
	for (auto &obj : mObjectList)
	{
		if (obj != nullptr)
		{
			delete(obj);
		}
	}
	return true;
}

void SceneBase::UpdateFadeIn(double t) {
	Update();
}

void SceneBase::UpdateFadeOut(double t) {
	Update();
}

void SceneBase::DrawFadeIn(double t)
{
	static bool first = true;
	// ここで真っ黒から透明へアルファ値を変化させながら画面サイズの矩形を描画する
	DirectX::XMFLOAT4 fadecolor = { 0,0,0,1 };

	DirectX::XMFLOAT2 uv[4] = {
		{0.0f,0.0f},
		{1.0f,0.0f},
		{0.0f,1.0f},
		{1.0f,1.0f}
	};

	if (first)
	{
		mQuadfadein.Init(Application::CLIENT_WIDTH,
						 Application::CLIENT_HEIGHT,
						 "assets/white.bmp", DirectX::XMFLOAT4(fadecolor.x, fadecolor.y, fadecolor.z, fadecolor.w));
		first = false;
	}

	//std::cout << "DrawFadeIn:" << t << std::endl;

	Render();

	fadecolor = mManager->GetFadeColor();

	fadecolor.w = fadecolor.w*(1.0f - static_cast<float>(t));

	TurnOffZbuffer();

	mQuadfadein.SetPosition(
		Application::CLIENT_WIDTH / 2.0f,
		Application::CLIENT_HEIGHT / 2.0f, 0);

	mQuadfadein.UpdateVertex(
		static_cast<uint32_t>(Application::CLIENT_WIDTH),
		static_cast<uint32_t>(Application::CLIENT_HEIGHT),
		DirectX::XMFLOAT4(fadecolor.x, fadecolor.y, fadecolor.z, fadecolor.w));

	mQuadfadein.UpdateVbuffer();
	mQuadfadein.Draw();

	TurnOnZbuffer();
}

void SceneBase::DrawFadeOut(double t)
{
	static bool first = true;
	// ここで真っ黒から透明へアルファ値を変化させながら画面サイズの矩形を描画する
	DirectX::XMFLOAT4 fadecolor = { 0,0,0,1 };
	DirectX::XMFLOAT2 uv[4] = {
		{0.0f,0.0f},
		{1.0f,0.0f},
		{0.0f,1.0f},
		{1.0f,1.0f}
	};

	if (first)
	{
		mQuadfadeout.Init(Application::CLIENT_WIDTH,
						  Application::CLIENT_HEIGHT,
						  "assets/white.bmp", DirectX::XMFLOAT4(fadecolor.x, fadecolor.y, fadecolor.z, fadecolor.w));
		first = false;
	}
	//std::cout << "DrawFadeOut:" << t << std::endl;

	TurnOnZbuffer();
	Render();
	TurnOffZbuffer();

	// ここで透明から真っ黒へアルファ値を変化させながら画面サイズの矩形を描画する
	fadecolor = mManager->GetFadeColor();

	fadecolor.w = static_cast<float>(t);

	mQuadfadeout.UpdateVertex(
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		DirectX::XMFLOAT4(fadecolor.x, fadecolor.y, fadecolor.z, fadecolor.w));

	mQuadfadeout.UpdateVbuffer();

	mQuadfadeout.SetPosition(
		static_cast<float>(Application::CLIENT_WIDTH / 2),
		static_cast<float>(Application::CLIENT_HEIGHT / 2), 0);

	mQuadfadeout.Draw();

	TurnOnZbuffer();
}