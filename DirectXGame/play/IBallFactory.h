#pragma once
#include "Ball.h"

class IBallFactory {
public:
    virtual ~IBallFactory() = default;
    virtual Ball* CreateBall(const KamataEngine::Vector3& position, int levelNumber) = 0;
};