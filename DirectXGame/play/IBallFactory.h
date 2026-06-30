#pragma once

#include "Ball.h"

#include <memory>

class IBallFactory {
public:
    virtual ~IBallFactory() = default;

    virtual std::unique_ptr<Ball> CreateBall(
        const KamataEngine::Vector3& position,
        int levelNumber
    ) = 0;
};