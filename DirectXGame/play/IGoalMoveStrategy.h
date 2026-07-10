#pragma once
#include "KamataEngine.h"
#include "GoalMovementConfig.h"


namespace MyEngine {

// ゴールの移動アルゴリズムを差し替えるための Strategy インターフェース
/// <summary>
/// Goal の移動アルゴリズムを差し替えるための Strategy インターフェース。
/// </summary>
class IGoalMoveStrategy {
public:
    virtual ~IGoalMoveStrategy() = default;

    // worldTransform を更新してゴールを動かす
    // timer は Goal 側で加算した値を渡す
    virtual void Apply(
        KamataEngine::WorldTransform& worldTransform,
        const KamataEngine::Vector3& initialPosition,
        float timer,
        const GoalMovementConfig& config
    ) = 0;
};

} // namespace MyEngine
