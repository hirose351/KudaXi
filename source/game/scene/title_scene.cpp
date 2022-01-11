#include	"title_scene.h"
#include	"../gameobject/ui_image.h"
#include	"../component/quad2d_component.h"
#include	"../component/model_component.h"
#include	"../component/billbord_component.h"
#include	"../gameobject/skydome.h"
#include	"../../system/dx11/CDirectInput.h"
#include	"maingame_scene.h"

//----- �萔��`
#define    BALL_QTY        2        // �����{�[����
#define BOUND_CONST        1.0f    // �͂˂�����W��

//----- �\����
struct BALL {
	float    x, y;        // �ʒu���W�i��ʒ��������_�Ax�E�{�Ay��{�j
	float    vx, vy;        // ���x�ipixel/s�j
	float    m;            // ����
	float    r;            // ���a�ipixel�j
};

BALL initBallData[BALL_QTY] = {
	//    x       y       vx      vy       m      r
	{ 0.0f,  20.0f, 20.0f, -10.0f, 200.0f, 4.0f },    // �{�[���O
	{ 0.0f, -20.0f, 20.0f,  10.0f, 200.0f, 4.0f },    // �{�[���P
};

BALL    g_Ball[BALL_QTY];    // �{�[���{��
float    before_length;                        // �O��̃{�[���Ԃ̎��ۂ̋���
float    length;                                // �{�[���Ԃ̎��ۂ̋���
float    dt = 16.0f / 1000.0f;                    // �������ԃ���
float    dx, dy;                                // ���W����
float    e = BOUND_CONST;                    // �͂˂�����W��
float    vx0_new = 0, vy0_new = 0, vx1_new = 0, vy1_new = 0;    // �Փˌ�̑��x
int        i;
int        out;                        // ��ʊO�ɏo���{�[���̐�

TitleScene::TitleScene()
{
	// �����ݒ�
	for (i = 0; i < BALL_QTY; i++)
	{
		g_Ball[i] = initBallData[i];
	}
	dx = g_Ball[1].x - g_Ball[0].x;        // �{�[���Ԃ̂w��������
	dy = g_Ball[1].y - g_Ball[0].y;        // �{�[���Ԃ̂x��������
	before_length = sqrtf(dx * dx + dy * dy);    // �����{�[���Ԃ̋������v�Z
}

TitleScene::~TitleScene()
{
}

void TitleScene::SceneAfter()
{
}

void TitleScene::SceneInit()
{
	Skydome* skydome = new Skydome;

	myUI::Image* rogo = new myUI::Image;
	rogo->GetTransform()->SetScale(Float3(700, 650, 0));
	rogo->GetTransform()->SetPositionXYZ(Float3(350, 350, 0));
	rogo->AddComponent<Component::Quad2d>()->SetInfo("assets/image/title/rogo.png", XMFLOAT4(1, 1, 1, 1));

	myUI::Image* i = new myUI::Image;
	i->GetTransform()->SetScale(Float3(300, 300, 0));
	i->GetTransform()->SetPositionXYZ(Float3(700, 500, 0));
	i->AddComponent<Component::Quad2d>()->SetInfo("assets/image/title/player.png", XMFLOAT4(1, 1, 1, 1), 4, 2);

	myUI::ButtonGroup* bG = new myUI::ButtonGroup;
	bG->GetTransform()->SetPositionXYZ(Float3(950.0f, 300.0f, 0));
	bG->SetInitState("assets/image/title/button.png", 2, 1, 1, ButtonTransition::eColorTint, XMFLOAT2(950.0f, 300.0f), XMFLOAT2(5, 5), XMFLOAT2(300, 300), XMFLOAT2(400, 400), ButtonArrangement::eVertical);
	bG->SetInitSelectNum(0);
	bG->SetPosition(0, XMFLOAT2(950.0f, 300.0f));
	bG->SetPosition(1, XMFLOAT2(1050, 500));
	mpBg = bG;

	Effect::Fruit* f = new Effect::Fruit;
	f->SetInit(Float3(0, 100, 0), 0);
	f->GetComponent<Component::Billbord>()->SetColor(XMFLOAT4(1, 1, 1, 0.5f));
	mpFruit = f;

	GameObject* obj = new GameObject(("GameObject"), ObjectType::eObstracle, true);
	bool sts = ModelMgr::GetInstance().LoadModel(
		"assets/model/player/playeranim.fbx",
		"shader/vs.hlsl", "shader/toonps.hlsl",
		"assets/model/player/");
	if (!sts)
	{
		MessageBox(nullptr, "Dice���f�� �ǂݍ��݃G���[", "error", MB_OK);
	}
	obj->AddComponent<Component::Model>()->SetModel(ModelMgr::GetInstance().GetModelPtr("assets/model/player/playeranim.fbx"));
	mpObj = obj;


	mCameraLookat = { 0,0,0 };
	CCamera::GetInstance()->SetLookat(mCameraLookat);
	CCamera::GetInstance()->SetEye(Float3(0, 0, -100));
	CCamera::GetInstance()->CreateCameraMatrix();
}

void TitleScene::SceneUpdate()
{
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_SPACE))
	{
		static int num = 0;
		mpFruit->SetInit(Float3(0, 100, 0), num);
		num = (num + 1) % 6;
	}

	// �{�[���̈ړ�
	for (i = out = 0; i < BALL_QTY; i++)
	{
		// �ړ�
		g_Ball[i].x += g_Ball[i].vx * dt;
		g_Ball[i].y += g_Ball[i].vy * dt;
		if ((g_Ball[i].x < -250 / 2.0f) || (g_Ball[i].x > 250 / 2.0f) ||
			(g_Ball[i].y < -100 / 2.0f) || (g_Ball[i].y > 100 / 2.0f))
		{    // ��ʊO�ɏo���
			out++;
		}
	}
	if (out >= BALL_QTY)
	{
		for (i = 0; i < BALL_QTY; i++)
		{
			g_Ball[i] = initBallData[i];
		}
	}

	// �{�[�����m�̃q�b�g�`�F�b�N
	dx = g_Ball[1].x - g_Ball[0].x;        // �{�[���Ԃ̂w��������
	dy = g_Ball[1].y - g_Ball[0].y;        // �{�[���Ԃ̂x��������
	length = sqrt(pow(dx, 2) + pow(dy, 2));                             // ��dx,dy���g���ă{�[���Ԃ̋������v�Z��length��

	if (initBallData[0].r + initBallData[1].r > length)
	{
		//�͂˂�����̖@�� e=-(v1'-v2')/(v1-v2) �Ɖ^���ʕۑ��� m1v1'+m2v2' = m1v1+m2v2 ��A��������
		vx0_new = ((initBallData[1].m*initBallData[1].vx)*(1 + e) + (-e * initBallData[1].m + initBallData[0].m)*initBallData[0].vx) / (initBallData[0].m + initBallData[1].m);    //��
		vy0_new = ((initBallData[1].m*initBallData[1].vy)*(1 + e) + (-e * initBallData[1].m + initBallData[0].m)*initBallData[0].vy) / (initBallData[0].m + initBallData[1].m);    //��
		vx1_new = ((initBallData[0].m*initBallData[0].vx)*(1 + e) + (-e * initBallData[0].m + initBallData[1].m)*initBallData[1].vx) / (initBallData[0].m + initBallData[1].m);    //��    
		vy1_new = ((initBallData[0].m*initBallData[0].vy)*(1 + e) + (-e * initBallData[0].m + initBallData[1].m)*initBallData[1].vy) / (initBallData[0].m + initBallData[1].m);    //��
		g_Ball[0].vx = vx0_new;
		g_Ball[0].vy = vy0_new;
		g_Ball[1].vx = vx1_new;
		g_Ball[1].vy = vy1_new;

	}    //if�̏I���
	before_length = sqrt(pow(g_Ball[1].x - g_Ball[0].x, 2) + pow(g_Ball[1].y - g_Ball[0].y, 2));    //�����݂̃{�[���ƃ{�[���̋�����ۑ�

	if (!mpBg->GetIsPressed())
		return;
	if (mpBg->GetSelectNum() == 0)
		SceneManager::GetInstance()->changeScene<MaingameScene>("GameMain", 1000, false);
	if (mpBg->GetSelectNum() == 1)
		SceneManager::GetInstance()->SetGameEndFlg(true);
}

void TitleScene::SceneRender()
{
	// �{�[����`��
	for (i = 0; i < BALL_QTY; i++)
	{
		mpObj->GetTransform()->SetPositionXYZ(Float3(g_Ball[i].x, g_Ball[i].y, 100));
		mpObj->Draw();
	}
}

bool TitleScene::Dispose()
{
	return false;
}
