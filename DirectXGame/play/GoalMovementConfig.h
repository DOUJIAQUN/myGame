#pragma once
#include "KamataEngine.h"


namespace MyEngine {

/// <summary>
/// Goal の移動方向を表す列挙型。
/// </summary>
enum class MoveDirection {
    Horizontal,  // 水平移动（左右）
    Vertical,    // 垂直移动（上下）
    Circular     // 圆形移动（可选扩展）
};

/// <summary>
/// Goal の移動有無、移動方向、移動範囲、移動速度をまとめて保持する構造体。
/// </summary>
struct GoalMovementConfig {
    bool shouldMove = false;           // 是否移动
    MoveDirection direction = MoveDirection::Horizontal; // 移动方向
    float moveRange = 10.0f;           // 移动范围
    float moveSpeed = 1.0f;            // 移动速度
    KamataEngine::Vector3 startPosition; // 起始位置

    // 构造函数
    GoalMovementConfig() = default;
    GoalMovementConfig(bool move, MoveDirection dir, float range, float speed)
        : shouldMove(move), direction(dir), moveRange(range), moveSpeed(speed) {
    }
};

} // namespace MyEngine
