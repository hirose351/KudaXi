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
	// �A�N�^�[���X�V���Ȃ�҂��Q�ɒǉ�
	if (mUpdatingActors || mInitingActors)
	{
		mPendingObjectList.emplace_back(_object);
	}
	else
	{
		mObjectList.emplace_back(_object);
	}
}

void SceneBase::RemoveGameObject(GameObject* _object)
{
	// �ۗ����̃A�N�^�[���ǂ���
	auto iter = std::find(mPendingObjectList.begin(), mPendingObjectList.end(), _object);
	if (iter != mPendingObjectList.end())
	{
		// �x�N�g���̍Ō�܂Ń��b�v���ă|�b�v�I�t���܂��i�R�s�[�̏����͔����Ă��������j
		std::iter_swap(iter, mPendingObjectList.end() - 1);
		mPendingObjectList.pop_back();
	}

	// ���݂���A�N�^�[���ǂ���
	iter = std::find(mObjectList.begin(), mObjectList.end(), _object);
	if (iter != mObjectList.end())
	{
		// �x�N�g���̍Ō�܂Ń��b�v���ă|�b�v�I�t���܂��i�R�s�[�̏����͔����Ă��������j
		std::iter_swap(iter, mObjectList.end() - 1);
		mObjectList.pop_back();
	}
}

bool SceneBase::Init()
{
	// ���ׂẴA�N�^�[���X�V
	mInitingActors = true;
	for (auto &obj : mObjectList)
	{
		obj->Init();
	}
	mInitingActors = false;

	// �҂��ɂȂ��Ă����A�N�^�[��m_actors�Ɉړ�
	for (auto pending : mPendingObjectList)
	{
		pending->Init();
		mObjectList.emplace_back(pending);
	}
	mPendingObjectList.clear();

	return true;
}

void SceneBase::Update()
{

	// ���ׂẴA�N�^�[���X�V
	mUpdatingActors = true;
	for (auto &obj : mObjectList)
	{
		if (!obj->GetIsStopPause() && !mIsPause)
			continue;
		obj->Update();
	}
	mUpdatingActors = false;

	// �҂��ɂȂ��Ă����A�N�^�[��m_actors�Ɉړ�
	for (auto pending : mPendingObjectList)
	{
		mObjectList.emplace_back(pending);
	}
	mPendingObjectList.clear();

	SceneUpdate();

	// ���񂾃A�N�^�[���ꎞ�z��ɒǉ�
	std::vector<GameObject*> deadObjcts;
	for (auto obj : mObjectList)
	{
		if (obj->GetObjectState() == ObjectState::eDead)
		{
			deadObjcts.emplace_back(obj);
		}
	}

	// ���񂾃A�N�^�[�������im_actors����폜�j
	for (auto obj : deadObjcts)
	{
		delete obj;
	}

	mObjectList.shrink_to_fit();
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
	// �����Ő^�������瓧���փA���t�@�l��ω������Ȃ����ʃT�C�Y�̋�`��`�悷��
	DirectX::XMFLOAT4 fadecolor = { 0,0,0,1 };

	DirectX::XMFLOAT2 uv[4] = {
		{0.0f,0.0f},
		{1.0f,0.0f},
		{0.0f,1.0f},
		{1.0f,1.0f}
	};

	if (first)
	{
		/*mQuadfadein.Init(Application::CLIENT_WIDTH,
						 Application::CLIENT_HEIGHT,
						 "assets/white.bmp", DirectX::XMFLOAT4(fadecolor.x, fadecolor.y, fadecolor.z, fadecolor.w));*/
		first = false;
	}

	//std::cout << "DrawFadeIn:" << t << std::endl;

	Render();

	fadecolor = mManager->GetFadeColor();

	fadecolor.w = fadecolor.w*(1.0f - static_cast<float>(t));

	TurnOffZbuffer();

	/*mQuadfadein.SetPosition(
		Application::CLIENT_WIDTH / 2.0f,
		Application::CLIENT_HEIGHT / 2.0f, 0);

	mQuadfadein.UpdateVertex(
		static_cast<uint32_t>(Application::CLIENT_WIDTH),
		static_cast<uint32_t>(Application::CLIENT_HEIGHT),
		DirectX::XMFLOAT4(fadecolor.x, fadecolor.y, fadecolor.z, fadecolor.w));

	mQuadfadein.UpdateVbuffer();
	mQuadfadein.Draw();*/

	TurnOnZbuffer();
}

void SceneBase::DrawFadeOut(double t)
{
	static bool first = true;
	// �����Ő^�������瓧���փA���t�@�l��ω������Ȃ����ʃT�C�Y�̋�`��`�悷��
	DirectX::XMFLOAT4 fadecolor = { 0,0,0,1 };
	DirectX::XMFLOAT2 uv[4] = {
		{0.0f,0.0f},
		{1.0f,0.0f},
		{0.0f,1.0f},
		{1.0f,1.0f}
	};

	if (first)
	{
		/*mQuadfadeout.Init(Application::CLIENT_WIDTH,
						  Application::CLIENT_HEIGHT,
						  "assets/white.bmp", DirectX::XMFLOAT4(fadecolor.x, fadecolor.y, fadecolor.z, fadecolor.w));*/
		first = false;
	}
	//std::cout << "DrawFadeOut:" << t << std::endl;

	TurnOnZbuffer();
	Render();
	TurnOffZbuffer();

	// �����œ�������^�����փA���t�@�l��ω������Ȃ����ʃT�C�Y�̋�`��`�悷��
	fadecolor = mManager->GetFadeColor();

	fadecolor.w = static_cast<float>(t);

	/*mQuadfadeout.UpdateVertex(
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		DirectX::XMFLOAT4(fadecolor.x, fadecolor.y, fadecolor.z, fadecolor.w));

	mQuadfadeout.UpdateVbuffer();

	mQuadfadeout.SetPosition(
		static_cast<float>(Application::CLIENT_WIDTH / 2),
		static_cast<float>(Application::CLIENT_HEIGHT / 2), 0);

	mQuadfadeout.Draw();*/

	TurnOnZbuffer();
}