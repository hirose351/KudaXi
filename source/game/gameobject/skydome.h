//#pragma once
//#include	"gameobject.h"
//#include	"../../system/model/CModel.h"
//
//class Skydome : public GameObject {
//private:
//	CModel*	mPmodel;	// ３Ｄモデル
//
//	// モデルポインタ取得
//	void SetModel(CModel* p) {
//		mPmodel = p;
//	}
//
//public:
//	enum Type {
//		TITLE,
//		GAME,
//		CLEATE,
//	};
//
//	Skydome();
//
//	void SetType(Type _type);
//
//	bool Init() override;		// 初期処理
//
//	void Draw();				// 描画
//
//	void Update();				// 更新
//
//	void Finalize() override;	// 終了処理	
//};