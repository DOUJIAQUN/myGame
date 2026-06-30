#include "ConcreteBallFactory.h"

#include <memory>

std::unique_ptr<Ball> ConcreteBallFactory::CreateBall(
    const KamataEngine::Vector3& position,
    int levelNumber
) {
    auto ball = std::make_unique<Ball>();

    // position や levelNumber は将来的に Ball の種類や初期パラメータを
    // 変えるために使用できる。
    // 現在は GameScene 側で Initialize / SetInitialPosition を行う。

    return ball;
}