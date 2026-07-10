#pragma once

#include "Ball.h"

#include <memory>

/// <summary>
/// Ball 生成処理を抽象化するための Factory インターフェース。
/// </summary>
class IBallFactory {
public:
    virtual ~IBallFactory() = default;

    virtual std::unique_ptr<MyEngine::Ball> CreateBall(
        const KamataEngine::Vector3& position,
        int levelNumber
    ) = 0;
};
