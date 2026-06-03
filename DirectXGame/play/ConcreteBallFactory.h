#pragma once
#include "IBallFactory.h"

class ConcreteBallFactory : public IBallFactory {
public:
    Ball* CreateBall(const KamataEngine::Vector3& position, int levelNumber) override;
};