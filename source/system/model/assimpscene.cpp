#include	<assimp/cimport.h>
#include	<assimp\postprocess.h>
#include	<assimp/cimport.h>
#include	<assimp\scene.h>

#include	"assimpscene.h"

bool AssimpScene::Init(std::string filename) {

	mScene = aiImportFile(filename.c_str(),
						  aiProcess_ConvertToLeftHanded |
						  aiProcessPreset_TargetRealtime_MaxQuality);

	if (mScene == nullptr)
	{
		return false;
	}

	return true;
}

// 終了処理
void AssimpScene::Exit() {
	aiReleaseImport(mScene);
}

// シーン取得
const aiScene* AssimpScene::GetScene() const {
	return mScene;
}

// アニメーションを持っているか
bool AssimpScene::HasAnimation() {
	return mScene->HasAnimations();
}