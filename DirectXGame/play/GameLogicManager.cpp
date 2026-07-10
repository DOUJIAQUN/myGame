#include "GameLogicManager.h"

#include <cmath>

using namespace KamataEngine;

namespace {
    constexpr float kBallCollisionRadius = 1.5f;
    constexpr float kGoalCollisionRadius = 1.5f;
    constexpr float kScreenBallRadius = 15.0f;
    constexpr int kLeftMouseButton = 0;
    constexpr float kExplosionKnockbackDuration = 0.6f;
    constexpr float kExplosionKnockbackMultiplier = 1.5f;
    constexpr float kCollisionDistanceScale = 2.0f;
    constexpr float kBallRadius = 2.0f;
    constexpr float kRestitution = 0.9f;
    constexpr float kImpulseScale = 1.2f;
    constexpr float kImpulseDistribution = 0.6f;
    constexpr float kCollisionKnockbackDuration = 0.6f;
    constexpr float kCollisionFriction = 50.0f;
    constexpr float kSeparationScale = 0.5f;
    constexpr float kNdcOffset = 1.0f;
    constexpr float kNdcToScreenScale = 0.5f;
    constexpr float kScreenWidth = 1280.0f;
    constexpr float kScreenHeight = 720.0f;
}

GameLogicManager::GameLogicManager()
    : balls_(nullptr),
    goals_(nullptr),
    camera_(nullptr),
    input_(Input::GetInstance()) {
}

void GameLogicManager::Initialize(
    std::vector<std::unique_ptr<MyEngine::Ball>>& balls,
    std::vector<std::unique_ptr<MyEngine::Goal>>& goals,
    Camera* camera
) {
    balls_ = &balls;
    goals_ = &goals;
    camera_ = camera;
    isGameOver_ = false;

    previousCollisionStates_.clear();
    previousCollisionStates_.resize(
        goals.size(),
        std::vector<bool>(balls.size(), false)
    );
}

void GameLogicManager::Update() {
    if (isGameOver_) {
        return;
    }

    if (!balls_ || !goals_ || !camera_) {
        return;
    }

    mousePos_ = input_->GetMousePosition();

    HandleMouseHover();

    if (input_->IsTriggerMouse(kLeftMouseButton)) {
        HandleMouseClick();
    }

    HandleBallCollisions();

    if (!isGameOver_) {
        isGameOver_ = CheckBallGoalCollision();
    }
}

void GameLogicManager::HandleMouseHover() {
    if (!balls_) {
        return;
    }

    // 重置所有球的鼠标悬停状态
    for (const auto& ball : *balls_) {
        if (ball) {
            ball->SetMouseOver(false);
        }
    }

    // 检测鼠标悬停
    for (const auto& ball : *balls_) {
        if (!ball) {
            continue;
        }

        if (ball->IsClickable() && IsMouseOverBall(ball.get(), mousePos_)) {
            ball->SetMouseOver(true);

            Vector3 screenPos = WorldToScreen(ball->GetPosition());
            ball->UpdateExplosionRangePosition(screenPos);

            break;
        }
    }
}

void GameLogicManager::HandleMouseClick() {
    if (!balls_) {
        return;
    }

    for (size_t i = 0; i < balls_->size(); i++) {
        MyEngine::Ball* clickedBall = (*balls_)[i].get();

        if (!clickedBall) {
            continue;
        }

        if (clickedBall->IsClickable() && IsMouseOverBall(clickedBall, mousePos_)) {
            clickedBall->Explode();

            Vector3 explosionPos = clickedBall->GetPosition();

            for (size_t j = 0; j < balls_->size(); j++) {
                if (i == j) {
                    continue;
                }

                MyEngine::Ball* otherBall = (*balls_)[j].get();

                if (!otherBall) {
                    continue;
                }

                if (otherBall->IsActive()) {
                    float distance = MyEngine::myMath::Distance(explosionPos, otherBall->GetPosition());

                    if (distance <= explosionRadius_) {
                        Vector3 direction = MyEngine::myMath::Subtract(otherBall->GetPosition(), explosionPos);
                        direction = MyEngine::myMath::Normalize(direction);

                        float distanceFactor = 1.0f - (distance / explosionRadius_);
                        Vector3 force = MyEngine::myMath::Multiply(
                            explosionForce_ * (1.0f + distanceFactor),
                            direction
                        );

                        otherBall->StartKnockback(
                            force,
                            kExplosionKnockbackDuration,
                            kExplosionKnockbackMultiplier,
                            true
                        );
                    }
                }
            }

            break;
        }
    }
}

bool GameLogicManager::CheckBallGoalCollision() {
    if (!goals_) {
        return false;
    }

    UpdateCompletionStatus();

    bool allCompleted = true;

    for (const auto& goal : *goals_) {
        if (!goal) {
            continue;
        }

        if (!goal->IsCompleted()) {
            allCompleted = false;
            break;
        }
    }

    return allCompleted;
}

void GameLogicManager::UpdateCompletionStatus() {
    if (!balls_ || !goals_) {
        return;
    }

    if (previousCollisionStates_.size() != goals_->size()) {
        previousCollisionStates_.assign(
            goals_->size(),
            std::vector<bool>(balls_->size(), false)
        );
    }

    for (size_t g = 0; g < goals_->size(); g++) {
        MyEngine::Goal* goal = (*goals_)[g].get();

        if (!goal) {
            continue;
        }

        if (g >= previousCollisionStates_.size()) {
            continue;
        }

        if (previousCollisionStates_[g].size() != balls_->size()) {
            previousCollisionStates_[g].assign(balls_->size(), false);
        }

        for (size_t b = 0; b < balls_->size(); b++) {
            MyEngine::Ball* ball = (*balls_)[b].get();

            if (!ball) {
                continue;
            }

            bool isColliding = ball->IsActive() && CheckCollisionBetweenBallAndGoal(ball, goal);
            bool wasColliding = previousCollisionStates_[g][b];

            if (isColliding && !wasColliding) {
                ball->OnEnterGoal(goal);
            }

            previousCollisionStates_[g][b] = isColliding;
        }
    }
}

bool GameLogicManager::CheckCollisionBetweenBallAndGoal(MyEngine::Ball* ball, MyEngine::Goal* goal) {
    if (!ball || !goal) {
        return false;
    }

    Vector3 ballPos = ball->GetPosition();
    Vector3 goalPos = goal->GetPosition();

    float distance = MyEngine::myMath::Distance(ballPos, goalPos);

    float collisionRadius = kBallCollisionRadius + kGoalCollisionRadius;
    return distance <= collisionRadius;
}

void GameLogicManager::HandleBallCollisions() {
    if (!balls_ || balls_->empty()) {
        return;
    }

    for (size_t i = 0; i < balls_->size(); i++) {
        MyEngine::Ball* ball1 = (*balls_)[i].get();

        if (!ball1 || !ball1->IsActive() || ball1->IsExploded()) {
            continue;
        }

        for (size_t j = i + 1; j < balls_->size(); j++) {
            MyEngine::Ball* ball2 = (*balls_)[j].get();

            if (!ball2 || !ball2->IsActive() || ball2->IsExploded()) {
                continue;
            }

            if (CheckBallBallCollision(ball1, ball2)) {
                ResolveBallCollision(ball1, ball2);
            }
        }
    }
}

bool GameLogicManager::CheckBallBallCollision(MyEngine::Ball* ball1, MyEngine::Ball* ball2) {
    if (!ball1 || !ball2) {
        return false;
    }

    Vector3 pos1 = ball1->GetPosition();
    Vector3 pos2 = ball2->GetPosition();

    float distance = MyEngine::myMath::Distance(pos1, pos2);

    float collisionDistance = kBallCollisionRadius * kCollisionDistanceScale;
    return distance <= collisionDistance;
}

void GameLogicManager::ResolveBallCollision(MyEngine::Ball* ball1, MyEngine::Ball* ball2) {
    if (!ball1 || !ball2) {
        return;
    }

    Vector3 pos1 = ball1->GetPosition();
    Vector3 pos2 = ball2->GetPosition();
    Vector3 vel1 = ball1->GetVelocity();
    Vector3 vel2 = ball2->GetVelocity();

    Vector3 collisionNormal = MyEngine::myMath::Subtract(pos2, pos1);
    float distance = MyEngine::myMath::Length(collisionNormal);

    if (distance == 0.0f) {
        return;
    }

    collisionNormal = MyEngine::myMath::Multiply(1.0f / distance, collisionNormal);

    Vector3 relativeVelocity = MyEngine::myMath::Subtract(vel1, vel2);
    float velocityAlongNormal = MyEngine::Dot(relativeVelocity, collisionNormal);

    if (velocityAlongNormal > 0) {
        return;
    }

    const float minDistance = kBallRadius * kCollisionDistanceScale;

    float impulseScalar = -(kNdcOffset + kRestitution) * velocityAlongNormal;
    impulseScalar *= kImpulseScale;

    Vector3 impulse1 = MyEngine::myMath::Multiply(-impulseScalar * kImpulseDistribution, collisionNormal);
    Vector3 impulse2 = MyEngine::myMath::Multiply(impulseScalar * kImpulseDistribution, collisionNormal);

    ball1->StartKnockback(
        impulse1,
        kCollisionKnockbackDuration,
        kCollisionFriction,
        false
    );

    ball2->StartKnockback(
        impulse2,
        kCollisionKnockbackDuration,
        kCollisionFriction,
        false
    );

    if (distance < minDistance && distance > 0.0f) {
        float overlap = minDistance - distance;
        Vector3 separation = MyEngine::myMath::Multiply(overlap * kSeparationScale, collisionNormal);

        ball1->SetPosition(MyEngine::myMath::Subtract(pos1, separation));
        ball2->SetPosition(MyEngine::myMath::Add(pos2, separation));
    }
}

bool GameLogicManager::IsMouseOverBall(MyEngine::Ball* ball, const Vector2& mousePos) {
    if (!ball) {
        return false;
    }

    Vector3 worldPos = ball->GetPosition();
    Vector3 screenPos = WorldToScreen(worldPos);

    float diffX = mousePos.x - screenPos.x;
    float diffY = mousePos.y - screenPos.y;

    float distance = static_cast<float>(
        std::sqrt(
            static_cast<double>(diffX * diffX + diffY * diffY)
        )
        );

    return distance <= kScreenBallRadius;
}

Vector3 GameLogicManager::WorldToScreen(const Vector3& worldPos) {
    if (!camera_) {
        return { 0.0f, 0.0f, 0.0f };
    }

    camera_->UpdateMatrix();

    const Matrix4x4& viewMatrix = camera_->matView;
    const Matrix4x4& projectionMatrix = camera_->matProjection;
    Matrix4x4 viewProjectionMatrix = MyEngine::myMath::Multiply(viewMatrix, projectionMatrix);

    Vector4 worldPos4 = {
        worldPos.x,
        worldPos.y,
        worldPos.z,
        kNdcOffset
    };

    Vector4 clipPos;

    clipPos.x =
        worldPos4.x * viewProjectionMatrix.m[0][0] +
        worldPos4.y * viewProjectionMatrix.m[1][0] +
        worldPos4.z * viewProjectionMatrix.m[2][0] +
        worldPos4.w * viewProjectionMatrix.m[3][0];

    clipPos.y =
        worldPos4.x * viewProjectionMatrix.m[0][1] +
        worldPos4.y * viewProjectionMatrix.m[1][1] +
        worldPos4.z * viewProjectionMatrix.m[2][1] +
        worldPos4.w * viewProjectionMatrix.m[3][1];

    clipPos.z =
        worldPos4.x * viewProjectionMatrix.m[0][2] +
        worldPos4.y * viewProjectionMatrix.m[1][2] +
        worldPos4.z * viewProjectionMatrix.m[2][2] +
        worldPos4.w * viewProjectionMatrix.m[3][2];

    clipPos.w =
        worldPos4.x * viewProjectionMatrix.m[0][3] +
        worldPos4.y * viewProjectionMatrix.m[1][3] +
        worldPos4.z * viewProjectionMatrix.m[2][3] +
        worldPos4.w * viewProjectionMatrix.m[3][3];

    if (clipPos.w != 0.0f) {
        clipPos.x /= clipPos.w;
        clipPos.y /= clipPos.w;
        clipPos.z /= clipPos.w;
    }

    float screenX = (clipPos.x + kNdcOffset) * kNdcToScreenScale * kScreenWidth;
    float screenY = (kNdcOffset - (clipPos.y + kNdcOffset) * kNdcToScreenScale) * kScreenHeight;

    return { screenX, screenY, clipPos.z };
}

void GameLogicManager::Reset() {
    isGameOver_ = false;
    currentGoalsReached_ = 0;

    if (balls_ && goals_) {
        previousCollisionStates_.clear();
        previousCollisionStates_.resize(
            goals_->size(),
            std::vector<bool>(balls_->size(), false)
        );
    }
}