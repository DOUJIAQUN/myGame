#pragma once

#include "KamataEngine.h"
#include "../play/Ball.h"
#include "../play/Goal.h"

#include <memory>
#include <vector>

/// <summary>
/// ゲーム中の入力判定、爆発処理、衝突判定、Goal 到達判定を管理するクラス。
/// </summary>
class GameLogicManager {
public:
    /// <summary>
    /// GameLogicManager に関する処理を行う。
    /// </summary>
    GameLogicManager();
    ~GameLogicManager() = default;

    // 初始化
    void Initialize(
        std::vector<std::unique_ptr<MyEngine::Ball>>& balls,
        std::vector<std::unique_ptr<MyEngine::Goal>>& goals,
        KamataEngine::Camera* camera
    );

    // 更新逻辑
    /// <summary>
    /// 毎フレームの更新処理を行う。
    /// </summary>
    void Update();

    // 碰撞检测
    /// <summary>
    /// CheckBallGoalCollision に関する処理を行う。
    /// </summary>
    bool CheckBallGoalCollision();
    /// <summary>
    /// CheckCollisionBetweenBallAndGoal に関する処理を行う。
    /// </summary>
    bool CheckCollisionBetweenBallAndGoal(MyEngine::Ball* ball, MyEngine::Goal* goal);

    int GetCurrentGoalsReached() const { return currentGoalsReached_; }
    int GetRequiredGoalsReached() const { return requiredGoalsReached_; }

    // 获取游戏状态
    bool IsGameOver() const { return isGameOver_; }
    void SetGameOver(bool gameOver) { isGameOver_ = gameOver; }

    // 重置游戏状态
    /// <summary>
    /// Reset に関する処理を行う。
    /// </summary>
    void Reset();

private:
    // 这里只是不拥有对象，只保存 GameScene 管理的对象容器地址
    std::vector<std::unique_ptr<MyEngine::Ball>>* balls_ = nullptr;
    std::vector<std::unique_ptr<MyEngine::Goal>>* goals_ = nullptr;

    KamataEngine::Camera* camera_ = nullptr;
    KamataEngine::Input* input_ = nullptr;

    // 游戏状态
    bool isGameOver_ = false;

    // 鼠标位置
    KamataEngine::Vector2 mousePos_ = { 0, 0 };

    // 爆炸参数
    const float explosionRadius_ = 10.1f;
    const float explosionForce_ = 1.1f;

    // 通关条件相关
    int requiredGoalsReached_ = 1;
    int currentGoalsReached_ = 0;

    // 上一帧的碰撞状态
    std::vector<std::vector<bool>> previousCollisionStates_;

private:
    /// <summary>
    /// UpdateCompletionStatus に関する処理を行う。
    /// </summary>
    void UpdateCompletionStatus();

    /// <summary>
    /// HandleMouseHover に関する処理を行う。
    /// </summary>
    void HandleMouseHover();
    /// <summary>
    /// HandleMouseClick に関する処理を行う。
    /// </summary>
    void HandleMouseClick();

    /// <summary>
    /// IsMouseOverBall に関する処理を行う。
    /// </summary>
    bool IsMouseOverBall(MyEngine::Ball* ball, const KamataEngine::Vector2& mousePos);
    /// <summary>
    /// WorldToScreen に関する処理を行う。
    /// </summary>
    KamataEngine::Vector3 WorldToScreen(const KamataEngine::Vector3& worldPos);

    // 球体间碰撞
    /// <summary>
    /// HandleBallCollisions に関する処理を行う。
    /// </summary>
    void HandleBallCollisions();
    /// <summary>
    /// CheckBallBallCollision に関する処理を行う。
    /// </summary>
    bool CheckBallBallCollision(MyEngine::Ball* ball1, MyEngine::Ball* ball2);
    /// <summary>
    /// ResolveBallCollision に関する処理を行う。
    /// </summary>
    void ResolveBallCollision(MyEngine::Ball* ball1, MyEngine::Ball* ball2);
};
