#pragma once

class GameScene;

class IGameState {
public:
    virtual ~IGameState() = default;
    virtual void Enter(GameScene* scene) = 0;   // 进入状态时的初始化
    virtual void Update(GameScene* scene) = 0;  // 每帧更新
    virtual void Exit(GameScene* scene) = 0;    // 离开状态时的清理
};