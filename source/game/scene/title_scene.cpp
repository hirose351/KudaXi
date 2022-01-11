#include	"title_scene.h"
#include	"../gameobject/ui_image.h"
#include	"../component/quad2d_component.h"
#include	"../component/model_component.h"
#include	"../component/billbord_component.h"
#include	"../gameobject/skydome.h"
#include	"../../system/dx11/CDirectInput.h"
#include	"maingame_scene.h"

//----- 定数定義
#define    BALL_QTY        2        // 初期ボール個数
#define BOUND_CONST        1.0f    // はねかえり係数

//----- 構造体
struct BALL {
	float    x, y;        // 位置座標（画面中央が原点、x右＋、y上＋）
	float    vx, vy;        // 速度（pixel/s）
	float    m;            // 質量
	float    r;            // 半径（pixel）
};

BALL initBallData[BALL_QTY] = {
	//    x       y       vx      vy       m      r
	{ 0.0f,  20.0f, 20.0f, -10.0f, 200.0f, 4.0f },    // ボール０
	{ 0.0f, -20.0f, 20.0f,  10.0f, 200.0f, 4.0f },    // ボール１
};

BALL    g_Ball[BALL_QTY];    // ボール本体
float    before_length;                        // 前回のボール間の実際の距離
float    length;                                // ボール間の実際の距離
float    dt = 16.0f / 1000.0f;                    // 微少時間Δｔ
float    dx, dy;                                // 座標差分
float    e = BOUND_CONST;                    // はねかえり係数
float    vx0_new = 0, vy0_new = 0, vx1_new = 0, vy1_new = 0;    // 衝突後の速度
int        i;
int        out;                        // 画面外に出たボールの数

TitleScene::TitleScene()
{
	// 初期設定
	for (i = 0; i < BALL_QTY; i++)
	{
		g_Ball[i] = initBallData[i];
	}
	dx = g_Ball[1].x - g_Ball[0].x;        // ボール間のＸ方向差分
	dy = g_Ball[1].y - g_Ball[0].y;        // ボール間のＹ方向差分
	before_length = sqrtf(dx * dx + dy * dy);    // 初期ボール間の距離を計算
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
		MessageBox(nullptr, "Diceモデル 読み込みエラー", "error", MB_OK);
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

	// ボールの移動
	for (i = out = 0; i < BALL_QTY; i++)
	{
		// 移動
		g_Ball[i].x += g_Ball[i].vx * dt;
		g_Ball[i].y += g_Ball[i].vy * dt;
		if ((g_Ball[i].x < -250 / 2.0f) || (g_Ball[i].x > 250 / 2.0f) ||
			(g_Ball[i].y < -100 / 2.0f) || (g_Ball[i].y > 100 / 2.0f))
		{    // 画面外に出れば
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

	// ボール同士のヒットチェック
	dx = g_Ball[1].x - g_Ball[0].x;        // ボール間のＸ方向差分
	dy = g_Ball[1].y - g_Ball[0].y;        // ボール間のＹ方向差分
	length = sqrt(pow(dx, 2) + pow(dy, 2));                             // ★dx,dyを使ってボール間の距離を計算しlengthへ

	if (initBallData[0].r + initBallData[1].r > length)
	{
		//はねかえりの法則 e=-(v1'-v2')/(v1-v2) と運動量保存則 m1v1'+m2v2' = m1v1+m2v2 を連立させる
		vx0_new = ((initBallData[1].m*initBallData[1].vx)*(1 + e) + (-e * initBallData[1].m + initBallData[0].m)*initBallData[0].vx) / (initBallData[0].m + initBallData[1].m);    //★
		vy0_new = ((initBallData[1].m*initBallData[1].vy)*(1 + e) + (-e * initBallData[1].m + initBallData[0].m)*initBallData[0].vy) / (initBallData[0].m + initBallData[1].m);    //★
		vx1_new = ((initBallData[0].m*initBallData[0].vx)*(1 + e) + (-e * initBallData[0].m + initBallData[1].m)*initBallData[1].vx) / (initBallData[0].m + initBallData[1].m);    //★    
		vy1_new = ((initBallData[0].m*initBallData[0].vy)*(1 + e) + (-e * initBallData[0].m + initBallData[1].m)*initBallData[1].vy) / (initBallData[0].m + initBallData[1].m);    //★
		g_Ball[0].vx = vx0_new;
		g_Ball[0].vy = vy0_new;
		g_Ball[1].vx = vx1_new;
		g_Ball[1].vy = vy1_new;

	}    //ifの終わり
	before_length = sqrt(pow(g_Ball[1].x - g_Ball[0].x, 2) + pow(g_Ball[1].y - g_Ball[0].y, 2));    //★現在のボールとボールの距離を保存

	if (!mpBg->GetIsPressed())
		return;
	if (mpBg->GetSelectNum() == 0)
		SceneManager::GetInstance()->changeScene<MaingameScene>("GameMain", 1000, false);
	if (mpBg->GetSelectNum() == 1)
		SceneManager::GetInstance()->SetGameEndFlg(true);
}

void TitleScene::SceneRender()
{
	// ボールを描画
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
