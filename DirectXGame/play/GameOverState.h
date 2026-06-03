#pragma once
#include "IGameState.h"

class GameOverState : public IGameState {
public:
    void Enter(GameScene* scene) override;
    void Update(GameScene* scene) override;
    void Exit(GameScene* scene) override;
};