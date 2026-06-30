#pragma once

#include "IBallFactory.h"

#include <memory>

class ConcreteBallFactory : public IBallFactory {
public:
    std::unique_ptr<Ball> CreateBall(
        const KamataEngine::Vector3& position,
        int levelNumber
    ) override;
};