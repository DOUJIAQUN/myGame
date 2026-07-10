#pragma once

#include "IBallFactory.h"

#include <memory>

/// <summary>
/// Ball オブジェクトの生成処理を具体的に実装する Factory クラス。
/// </summary>
class ConcreteBallFactory : public IBallFactory {
public:
    std::unique_ptr<MyEngine::Ball> CreateBall(
        const KamataEngine::Vector3& position,
        int levelNumber
    ) override;
};
