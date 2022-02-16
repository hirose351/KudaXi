#pragma once
#include	<memory>
#include	<wrl/client.h>
#include	<unordered_map>
#include	"CModel.h"
#include	"../util/uncopyable.h"

using Microsoft::WRL::ComPtr;

class ModelMgr :public Uncopyable {
private:
	ModelMgr() {
	}
	std::unordered_map<
		std::string,					// キー
		std::unique_ptr<CModel>			// データ
	> mModelhashmap;

public:
	static ModelMgr& GetInstance() {
		static ModelMgr Instance;
		return Instance;
	}

	virtual ~ModelMgr() {
		Finalize();
	}

	// モデルの後始末
	void Finalize();

	// モデルをロードする
	bool LoadModel(std::string _modelfilename,
				   std::string _vsfilename,
				   std::string _psfilename,
				   std::string _texfilefolder);

	// モデルのポインタを取得する
	CModel* GetModelPtr(std::string _key);
};