#pragma once
#include "KamataEngine.h"
#include "../play/Ball.h"
#include "../play/Goal.h"
#include <vector>

class GameLogicManager {
public:
    GameLogicManager();
    ~GameLogicManager() = default;

    // 初始化
    void Initialize(std::vector<Ball*>& balls, std::vector<Goal*>& goals, KamataEngine::Camera* camera);

    // 更新逻辑
    void Update();

    // 碰撞检测
    bool CheckBallGoalCollision();

    // 获取游戏状态
    bool IsGameOver() const { return isGameOver_; }
    void SetGameOver(bool gameOver) { isGameOver_ = gameOver; }

    // 重置游戏状态
    void Reset();

private:
    // 引用游戏对象
    std::vector<Ball*>* balls_;
    std::vector<Goal*>* goals_;
    KamataEngine::Camera* camera_;
    KamataEngine::Input* input_;

    // 游戏状态
    bool isGameOver_ = false;

    // 鼠标位置
    KamataEngine::Vector2 mousePos_ = { 0, 0 };

    // 爆炸参数
    const float explosionRadius_ = 10.1f;
    const float explosionForce_ = 1.0f;

    // 私有方法
    void HandleMouseHover();
    void HandleMouseClick();
    bool IsMouseOverBall(Ball* ball, const KamataEngine::Vector2& mousePos);
    KamataEngine::Vector3 WorldToScreen(const KamataEngine::Vector3& worldPos);
    bool CheckCollisionBetweenBallAndGoal(Ball* ball, Goal* goal);
};