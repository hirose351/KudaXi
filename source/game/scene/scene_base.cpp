#include	"scene_base.h"
#include	"../gameobject/gameobject.h"
#include	"../../system/imgui/util/myimgui.h"

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
	objectList.clear();
	objectList.shrink_to_fit();
}

void SceneBase::AddGameObject(GameObject* _object)
{
	objectList.emplace_back(_object);
}

bool SceneBase::Init()
{
	for (auto &obj : objectList)
	{
		obj->Init();
	}
	return true;
}

void SceneBase::Update()
{
	for (auto &obj : objectList)
	{
		obj->Update();
	}
	SceneUpdate();
}

void SceneBase::Render()
{
	for (auto &obj : objectList)
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
	for (auto &obj : objectList)
	{
		obj->ImguiDraw();
	}
	ImGui::End();

}

bool SceneBase::Dispose()
{
	for (auto &obj : objectList)
	{
		if (obj != nullptr)
		{
			delete(obj);
		}
	}
	return true;
}

void SceneBase::updateFadeIn(double t) {
	Update();
}

void SceneBase::updateFadeOut(double t) {
	Update();
}

void SceneBase::drawFadeIn(double t)
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
		gQuadfadein.Init(Application::CLIENT_WIDTH,
						 Application::CLIENT_HEIGHT,
						 "assets/white.bmp", DirectX::XMFLOAT4(fadecolor.x, fadecolor.y, fadecolor.z, fadecolor.w));
		first = false;
	}

	//std::cout << "DrawFadeIn:" << t << std::endl;

	Render();

	fadecolor = mManager->GetFadeColor();

	fadecolor.w = fadecolor.w*(1.0f - static_cast<float>(t));

	TurnOffZbuffer();

	gQuadfadein.SetPosition(
		Application::CLIENT_WIDTH / 2.0f,
		Application::CLIENT_HEIGHT / 2.0f, 0);

	gQuadfadein.UpdateVertex(
		static_cast<uint32_t>(Application::CLIENT_WIDTH),
		static_cast<uint32_t>(Application::CLIENT_HEIGHT),
		DirectX::XMFLOAT4(fadecolor.x, fadecolor.y, fadecolor.z, fadecolor.w));

	gQuadfadein.UpdateVbuffer();
	gQuadfadein.Draw();

	TurnOnZbuffer();
}

void SceneBase::drawFadeOut(double t)
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
		gQuadfadeout.Init(Application::CLIENT_WIDTH,
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

	gQuadfadeout.UpdateVertex(
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		DirectX::XMFLOAT4(fadecolor.x, fadecolor.y, fadecolor.z, fadecolor.w));

	gQuadfadeout.UpdateVbuffer();

	gQuadfadeout.SetPosition(
		static_cast<float>(Application::CLIENT_WIDTH / 2),
		static_cast<float>(Application::CLIENT_HEIGHT / 2), 0);

	gQuadfadeout.Draw();

	TurnOnZbuffer();
}