#include "GameLogicManager.h"
#include <cmath>

using namespace KamataEngine;

namespace {
    // 碰撞相关常量
    const float BALL_COLLISION_RADIUS = 1.5f;      // 3D空间中的球体碰撞半径
    const float GOAL_COLLISION_RADIUS = 1.5f;      // 3D空间中的终点碰撞半径
    const float SCREEN_BALL_RADIUS = 15.0f;        // 屏幕坐标中的球体半径（用于鼠标检测）
}


GameLogicManager::GameLogicManager()
    : balls_(nullptr), goals_(nullptr), camera_(nullptr) {
    input_ = Input::GetInstance();
}

void GameLogicManager::Initialize(std::vector<Ball*>& balls, std::vector<Goal*>& goals, Camera* camera) {
    balls_ = &balls;
    goals_ = &goals;
    camera_ = camera;
    isGameOver_ = false;

    // 初始化碰撞状态记录
    previousCollisionStates_.resize(goals.size(), std::vector<bool>(balls.size(), false));
}

void GameLogicManager::Update() {
    if (isGameOver_) return;

    // 获取鼠标位置
    mousePos_ = input_->GetMousePosition();

    // 处理鼠标悬停
    HandleMouseHover();

    // 处理鼠标点击
    if (input_->IsTriggerMouse(0)) {
        HandleMouseClick();
    }

    // 处理球体间的碰撞（新增）
    HandleBallCollisions();

    // 检测小球与终点的碰撞
    if (!isGameOver_) {
        isGameOver_ = CheckBallGoalCollision();
    }
}

void GameLogicManager::HandleMouseHover() {
    // 重置所有球的鼠标悬停状态
    for (Ball* ball : *balls_) {
        ball->SetMouseOver(false);
    }

    // 检测鼠标悬停
    for (Ball* ball : *balls_) {
        if (ball->IsClickable()) {
            if (IsMouseOverBall(ball, mousePos_)) {
                ball->SetMouseOver(true);

                // 更新爆炸范围位置
                Vector3 screenPos = WorldToScreen(ball->GetPosition());
                ball->UpdateExplosionRangePosition(screenPos);

                break; // 只处理第一个悬停的球体
            }
        }
    }
}

void GameLogicManager::HandleMouseClick() {
    // 遍历所有球体，检查鼠标是否点击到球体
    for (size_t i = 0; i < balls_->size(); i++) {
        Ball* clickedBall = (*balls_)[i];

        // 检查球体是否活跃且未被爆炸
        if (clickedBall->IsClickable()) {
            // 使用IsMouseOverBall检测鼠标是否在球体范围内
            if (IsMouseOverBall(clickedBall, mousePos_)) {
                // 处理鼠标点击到小球的情况
                clickedBall->Explode();

                // 获取爆炸位置
                Vector3 explosionPos = clickedBall->GetPosition();

                // 对范围内的其他球体施加爆炸力
                for (size_t j = 0; j < balls_->size(); j++) {
                    if (i == j) continue; // 跳过被点击的球体本身

                    Ball* otherBall = (*balls_)[j];
                    if (otherBall->IsActive()) {
                        float distance = myMath::Distance(explosionPos, otherBall->GetPosition());

                        if (distance <= explosionRadius_) {
                            // 计算爆炸方向（从爆炸中心指向球体）
                            Vector3 direction = myMath::Subtract(otherBall->GetPosition(), explosionPos);
                            direction = myMath::Normalize(direction);

                            // 根据距离调整爆炸力（越近力越大）
                            float distanceFactor = 1.0f - (distance / explosionRadius_);
                            Vector3 force = myMath::Multiply(explosionForce_ * (1.0f + distanceFactor), direction);

                            // 使用统一的击飞方法，爆炸击飞使用更强参数
                            otherBall->StartKnockback(force,
                                0.6f,     // 较短但更强烈的击飞
                                1.5f,     // 更强的力倍增
                                true);    // 是爆炸击飞
                        }
                    }
                }
                break; // 只处理第一个被点击的球体
            }
        }
    }
}

bool GameLogicManager::CheckBallGoalCollision() {
    UpdateCompletionStatus();
    // 检查是否所有终点都完成了需求次数
    bool allCompleted = true;
    for (Goal* goal : *goals_) {
        if (!goal->IsCompleted()) {
            allCompleted = false;
            break;
        }
    }

    return allCompleted;
}

// 新增更新完成状态的方法
void GameLogicManager::UpdateCompletionStatus() {
    // 检查每个球和每个终点的碰撞
    for (size_t g = 0; g < goals_->size(); g++) {
        Goal* goal = (*goals_)[g];

        for (size_t b = 0; b < balls_->size(); b++) {
            Ball* ball = (*balls_)[b];
            // 检查球体是否有效
            if (!ball) continue;

            bool isColliding = ball->IsActive() && CheckCollisionBetweenBallAndGoal(ball, goal);
            bool wasColliding = previousCollisionStates_[g][b];

            // 如果当前帧碰撞而上一帧没有碰撞，说明是刚进入
            if (isColliding && !wasColliding) {
                // 球体刚进入终点，增加计数
                goal->IncrementCount();

                ball->SetActive(false);

                ball->SetMouseOver(false);

                previousCollisionStates_[g][b] = false;
            }

            // 更新碰撞状态
            previousCollisionStates_[g][b] = isColliding;
        }
    }
}

bool GameLogicManager::CheckCollisionBetweenBallAndGoal(Ball* ball, Goal* goal) {
    // 获取球体和目标的位置
    Vector3 ballPos = ball->GetPosition();
    Vector3 goalPos = goal->GetPosition();

    // 计算距离
    float distance = myMath::Distance(ballPos, goalPos);

    // 碰撞半径（根据球体和目标的大小调整）
    float collisionRadius = BALL_COLLISION_RADIUS + GOAL_COLLISION_RADIUS;
    return distance <= collisionRadius;
}

// 处理球体间碰撞的方法
void GameLogicManager::HandleBallCollisions() {
    if (!balls_ || balls_->empty()) return;

    // 遍历所有球体对，检测碰撞
    for (size_t i = 0; i < balls_->size(); i++) {
        Ball* ball1 = (*balls_)[i];

        // 只检查活跃且未爆炸的球体
        if (!ball1 || !ball1->IsActive() || ball1->IsExploded()) {
            continue;
        }

        for (size_t j = i + 1; j < balls_->size(); j++) {
            Ball* ball2 = (*balls_)[j];

            // 只检查活跃且未爆炸的球体
            if (!ball2 || !ball2->IsActive() || ball2->IsExploded()) {
                continue;
            }

            // 检测碰撞
            if (CheckBallBallCollision(ball1, ball2)) {
                ResolveBallCollision(ball1, ball2);
            }
        }
    }
}

// 检测两个球体是否碰撞
bool GameLogicManager::CheckBallBallCollision(Ball* ball1, Ball* ball2) {
    Vector3 pos1 = ball1->GetPosition();
    Vector3 pos2 = ball2->GetPosition();

    // 计算距离
    float distance = myMath::Distance(pos1, pos2);

    // 碰撞半径（根据球体大小调整）
    float collisionDistance = BALL_COLLISION_RADIUS * 2.0f;
    return distance <= collisionDistance;
}

// 解析碰撞，应用物理逻辑
void GameLogicManager::ResolveBallCollision(Ball* ball1, Ball* ball2) {
    Vector3 pos1 = ball1->GetPosition();
    Vector3 pos2 = ball2->GetPosition();
    Vector3 vel1 = ball1->GetVelocity();
    Vector3 vel2 = ball2->GetVelocity();

    // 计算碰撞法线
    Vector3 collisionNormal = myMath::Subtract(pos2, pos1);
    float distance = myMath::Length(collisionNormal);

    if (distance == 0.0f) return;

    collisionNormal = myMath::Multiply(1.0f / distance, collisionNormal);

    // 计算相对速度
    Vector3 relativeVelocity = myMath::Subtract(vel1, vel2);
    float velocityAlongNormal = Dot(relativeVelocity, collisionNormal);

    // 如果球体正在分开，不需要处理
    if (velocityAlongNormal > 0) {
        return;
    }

    // 碰撞参数
    const float BALL_RADIUS = 2.0f;
    const float MIN_DISTANCE = BALL_RADIUS * 2.0f;

    // 恢复系数（弹性）
    float restitution = 0.9f; // 从0.8增加到0.9，更有弹性

    // 计算冲量（增加碰撞力）
    float impulseScalar = -(1.0f + restitution) * velocityAlongNormal;
    impulseScalar *= 1.2f; // 增加20%的击飞力

    // 应用冲量（更强的击飞）
    Vector3 impulse1 = myMath::Multiply(-impulseScalar * 0.6f, collisionNormal);
    Vector3 impulse2 = myMath::Multiply(impulseScalar * 0.6f, collisionNormal);

    // 使用统一的击飞方法，相撞击飞使用不同参数
    ball1->StartKnockback(impulse1,
        0.6f,     // 持续时间稍长
        50.0f,     // 力倍增器
        false);   // 不是爆炸击飞

    ball2->StartKnockback(impulse2,
        0.6f,
        50.0f,
        false);

    // 分离球体，防止重叠
    if (distance < MIN_DISTANCE && distance > 0.0f) {
        float overlap = MIN_DISTANCE - distance;
        Vector3 separation = myMath::Multiply(overlap * 0.5f, collisionNormal);

        ball1->SetPosition(myMath::Subtract(pos1, separation));
        ball2->SetPosition(myMath::Add(pos2, separation));
    }
}

bool GameLogicManager::IsMouseOverBall(Ball* ball, const Vector2& mousePos) {
    // 获取球体的世界坐标
    Vector3 worldPos = ball->GetPosition();

    // 将世界坐标转换为屏幕坐标
    Vector3 screenPos = WorldToScreen(worldPos);

   

    // 计算鼠标位置与球体屏幕位置的距离
    float distance = std::sqrt(std::pow(mousePos.x - screenPos.x, 2) + std::pow(mousePos.y - screenPos.y, 2));

    // 圆形碰撞检测：检查鼠标是否在圆形范围内
    return distance <= SCREEN_BALL_RADIUS;
}

Vector3 GameLogicManager::WorldToScreen(const Vector3& worldPos) {
    // 确保相机矩阵已经更新
    camera_->UpdateMatrix();

    // 使用相机的视图投影矩阵进行准确的坐标转换
    const Matrix4x4& viewMatrix = camera_->matView;
    const Matrix4x4& projectionMatrix = camera_->matProjection;
    Matrix4x4 viewProjectionMatrix = myMath::Multiply(viewMatrix, projectionMatrix);

    // 变换到齐次裁剪空间
    Vector4 worldPos4 = { worldPos.x, worldPos.y, worldPos.z, 1.0f };

    // 手动进行矩阵变换
    Vector4 clipPos;
    clipPos.x = worldPos4.x * viewProjectionMatrix.m[0][0] + worldPos4.y * viewProjectionMatrix.m[1][0] + worldPos4.z * viewProjectionMatrix.m[2][0] + worldPos4.w * viewProjectionMatrix.m[3][0];
    clipPos.y = worldPos4.x * viewProjectionMatrix.m[0][1] + worldPos4.y * viewProjectionMatrix.m[1][1] + worldPos4.z * viewProjectionMatrix.m[2][1] + worldPos4.w * viewProjectionMatrix.m[3][1];
    clipPos.z = worldPos4.x * viewProjectionMatrix.m[0][2] + worldPos4.y * viewProjectionMatrix.m[1][2] + worldPos4.z * viewProjectionMatrix.m[2][2] + worldPos4.w * viewProjectionMatrix.m[3][2];
    clipPos.w = worldPos4.x * viewProjectionMatrix.m[0][3] + worldPos4.y * viewProjectionMatrix.m[1][3] + worldPos4.z * viewProjectionMatrix.m[2][3] + worldPos4.w * viewProjectionMatrix.m[3][3];

    // 透视除法
    if (clipPos.w != 0.0f) {
        clipPos.x /= clipPos.w;
        clipPos.y /= clipPos.w;
        clipPos.z /= clipPos.w;
    }

    // 转换到屏幕坐标
    // 假设屏幕分辨率为1280x720
    float screenX = (clipPos.x + 1.0f) * 0.5f * 1280.0f;
    float screenY = (1.0f - (clipPos.y + 1.0f) * 0.5f) * 720.0f;

    return { screenX, screenY, clipPos.z };
}

void GameLogicManager::Reset() {
    isGameOver_ = false;
}