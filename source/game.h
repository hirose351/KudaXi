#pragma once
#include <cinttypes>

void GameInit();
void GameSceneInit();
void GameInput(uint64_t dt);
void GameUpdate(uint64_t dt);
void GameRender(uint64_t dt);
void GameDispose();

#include <vector>

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	//void AddSprite(class SpriteComponent* sprite);
	//void RemoveSprite(class SpriteComponent* sprite);

	//SDL_Texture* GetTexture(const std::string& fileName);
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// Map of textures loaded
	//std::unordered_map<std::string, SDL_Texture*> mTextures;

	// �A�N�e�B�u�ȃA�N�^�[�Q
	std::vector<class Actor*> mActors;
	// �҂���Ԃ̃A�N�^�[�Q
	std::vector<class Actor*> mPendingActors;

	// All the sprite components drawn
	std::vector<class SpriteComponent*> mSprites;

	bool mIsRunning;
	// �A�N�^�[���X�V���Ă��邩�ǂ���
	bool mUpdatingActors;

	// Game-specific
	class Ship* mShip; // Player's ship
};
