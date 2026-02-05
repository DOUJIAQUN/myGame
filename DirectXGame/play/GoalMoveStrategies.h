#pragma once
#include <cmath>
#include "IGoalMoveStrategy.h"

// 何もしない（移動なし）
class NoMoveStrategy final : public IGoalMoveStrategy {
public:
    void Apply(
        KamataEngine::WorldTransform& /*worldTransform*/,
        const KamataEngine::Vector3& /*initialPosition*/,
        float /*timer*/,
        const GoalMovementConfig& /*config*/
    ) override {
        // do nothing
    }
};

// 水平移動（左右）
class HorizontalMoveStrategy final : public IGoalMoveStrategy {
public:
    void Apply(
        KamataEngine::WorldTransform& worldTransform,
        const KamataEngine::Vector3& initialPosition,
        float timer,
        const GoalMovementConfig& config
    ) override {
        float moveDistance = std::sin(timer * config.moveSpeed) * config.moveRange;
        worldTransform.translation_.x = initialPosition.x + moveDistance;
    }
};

// 垂直移動（上下）
class VerticalMoveStrategy final : public IGoalMoveStrategy {
public:
    void Apply(
        KamataEngine::WorldTransform& worldTransform,
        const KamataEngine::Vector3& initialPosition,
        float timer,
        const GoalMovementConfig& config
    ) override {
        float moveDistance = std::sin(timer * config.moveSpeed) * config.moveRange;
        worldTransform.translation_.y = initialPosition.y + moveDistance;
    }
};

// 円運動
class CircularMoveStrategy final : public IGoalMoveStrategy {
public:
    void Apply(
        KamataEngine::WorldTransform& worldTransform,
        const KamataEngine::Vector3& initialPosition,
        float timer,
        const GoalMovementConfig& config
    ) override {
        worldTransform.translation_.x =
            initialPosition.x + std::cos(timer * config.moveSpeed) * config.moveRange;
        worldTransform.translation_.y =
            initialPosition.y + std::sin(timer * config.moveSpeed) * config.moveRange;
    }
};
