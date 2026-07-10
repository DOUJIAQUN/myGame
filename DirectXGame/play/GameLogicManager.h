#pragma once

#include "KamataEngine.h"
#include "../play/Ball.h"
#include "../play/Goal.h"

#include <memory>
#include <vector>


namespace MyEngine {

/// <summary>
/// ゲーム中の入力判定、爆発処理、衝突判定、Goal 到達判定を管理するクラス。
/// </summary>
class GameLogicManager {
public:
    /// <summary>
/// GameScene が所有する Ball / Goal の配列とカメラを登録し、判定状態を初期化する。
/// </summary>
/// <param name="balls">GameScene が所有する Ball 配列。所有権は受け取らない。</param>
/// <param name="goals">GameScene が所有する Goal 配列。所有権は受け取らない。</param>
/// <param name="camera">マウス判定用の座標変換に使用するカメラ。</param>
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
/// すべての Goal の達成状態を更新し、ステージクリア済みかどうかを返す。
/// </summary>
    bool CheckBallGoalCollision();
    /// <summary>
/// Ball と Goal の中心距離を調べ、接触しているかを返す。
/// </summary>
/// <param name="ball">判定対象の Ball。</param>
/// <param name="goal">判定対象の Goal。</param>
/// <returns>接触半径内に入っていれば true。</returns>
    bool CheckCollisionBetweenBallAndGoal(MyEngine::Ball* ball, MyEngine::Goal* goal);

    int GetCurrentGoalsReached() const { return currentGoalsReached_; }
    int GetRequiredGoalsReached() const { return requiredGoalsReached_; }

    // 获取游戏状态
    bool IsGameOver() const { return isGameOver_; }
    void SetGameOver(bool gameOver) { isGameOver_ = gameOver; }

    // 重置游戏状态
    /// <summary>
/// Ball の爆発状態、速度、座標、トレイルを初期状態へ戻す。
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
/// 前フレームの接触状態と比較し、Goal へ新しく入った Ball だけをカウントする。
/// </summary>
    void UpdateCompletionStatus();

    /// <summary>
/// マウス位置に最も近いクリック可能な Ball を判定し、爆発範囲表示を更新する。
/// </summary>
    void HandleMouseHover();
    /// <summary>
/// クリックされた Ball を爆発させ、範囲内の他の Ball へ距離に応じた力を加える。
/// </summary>
    void HandleMouseClick();

    /// <summary>
/// 指定した Ball がマウスカーソルの選択範囲内にあるか判定する。
/// </summary>
/// <param name="ball">判定対象の Ball。</param>
/// <param name="mousePos">現在のマウス座標。</param>
/// <returns>マウスが Ball の画面上半径内にあれば true。</returns>
    bool IsMouseOverBall(MyEngine::Ball* ball, const KamataEngine::Vector2& mousePos);
    /// <summary>
/// 3D ワールド座標を UI 判定に使うスクリーン座標へ変換する。
/// </summary>
/// <param name="worldPos">変換したい 3D ワールド座標。</param>
/// <returns>x, y に画面座標、z に奥行きを持つ Vector3。</returns>
    KamataEngine::Vector3 WorldToScreen(const KamataEngine::Vector3& worldPos);

    // 球体间碰撞
    /// <summary>
/// 有効な Ball の全組み合わせを調べ、重なっているものだけ衝突解決する。
/// </summary>
    void HandleBallCollisions();
    /// <summary>
/// 2つの Ball の中心距離を使い、半径同士が重なっているか判定する。
/// </summary>
    bool CheckBallBallCollision(MyEngine::Ball* ball1, MyEngine::Ball* ball2);
    /// <summary>
/// 衝突法線と相対速度から反発方向を求め、2つの Ball を押し戻す。
/// </summary>
    void ResolveBallCollision(MyEngine::Ball* ball1, MyEngine::Ball* ball2);
};

} // namespace MyEngine
