#pragma once
#include <cinttypes>

void GameInit();
void GameSceneInit();
void GameInput(uint64_t dt);
void GameUpdate(uint64_t dt);
void GameRender(uint64_t dt);
void GameDispose();