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

    // 修改碰撞检测方法
    bool CheckBallGoalCollision();  // 现在返回是否满足通关条件
    bool CheckCollisionBetweenBallAndGoal(Ball* ball, Goal* goal);  // 单个碰撞检测

   
    int GetCurrentGoalsReached() const { return currentGoalsReached_; }
    int GetRequiredGoalsReached() const { return requiredGoalsReached_; }

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

    // 新增通关条件相关变量
    int requiredGoalsReached_ = 1;
    int currentGoalsReached_ = 0;

    std::vector<std::vector<bool>> previousCollisionStates_; // 上一帧的碰撞状态

    void UpdateCompletionStatus();  // 更新完成状态

    // 私有方法
    void HandleMouseHover();
    void HandleMouseClick();
    bool IsMouseOverBall(Ball* ball, const KamataEngine::Vector2& mousePos);
    KamataEngine::Vector3 WorldToScreen(const KamataEngine::Vector3& worldPos);
   
};