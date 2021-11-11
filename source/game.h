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

	// アクティブなアクター群
	std::vector<class Actor*> mActors;
	// 待ち状態のアクター群
	std::vector<class Actor*> mPendingActors;

	// All the sprite components drawn
	std::vector<class SpriteComponent*> mSprites;

	bool mIsRunning;
	// アクターを更新しているかどうか
	bool mUpdatingActors;

	// Game-specific
	class Ship* mShip; // Player's ship
};
