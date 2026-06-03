#include "ConcreteBallFactory.h"
#include "Ball.h"

Ball* ConcreteBallFactory::CreateBall(const KamataEngine::Vector3& position, int levelNumber) {
    Ball* ball = new Ball();
    // 这里可以根据 levelNumber 或其它条件设置球的不同属性
    // 例如：level 越高，球越大或速度越快
    // 但目前 Ball 没有派生类，我们仅展示工厂模式的结构
    // 未来可在此处返回不同的 Ball 子类
    return ball;
}