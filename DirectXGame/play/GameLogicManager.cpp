#include "GameLogicManager.h"

#include <cmath>

using namespace KamataEngine;

namespace {
    // 碰撞相关常量
    const float BALL_COLLISION_RADIUS = 1.5f;
    const float GOAL_COLLISION_RADIUS = 1.5f;
    const float SCREEN_BALL_RADIUS = 15.0f;
}

GameLogicManager::GameLogicManager()
    : balls_(nullptr),
    goals_(nullptr),
    camera_(nullptr),
    input_(Input::GetInstance()) {
}

void GameLogicManager::Initialize(
    std::vector<std::unique_ptr<Ball>>& balls,
    std::vector<std::unique_ptr<Goal>>& goals,
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

    if (input_->IsTriggerMouse(0)) {
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
        Ball* clickedBall = (*balls_)[i].get();

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

                Ball* otherBall = (*balls_)[j].get();

                if (!otherBall) {
                    continue;
                }

                if (otherBall->IsActive()) {
                    float distance = myMath::Distance(explosionPos, otherBall->GetPosition());

                    if (distance <= explosionRadius_) {
                        Vector3 direction = myMath::Subtract(otherBall->GetPosition(), explosionPos);
                        direction = myMath::Normalize(direction);

                        float distanceFactor = 1.0f - (distance / explosionRadius_);
                        Vector3 force = myMath::Multiply(
                            explosionForce_ * (1.0f + distanceFactor),
                            direction
                        );

                        otherBall->StartKnockback(
                            force,
                            0.6f,
                            1.5f,
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
        Goal* goal = (*goals_)[g].get();

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
            Ball* ball = (*balls_)[b].get();

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

bool GameLogicManager::CheckCollisionBetweenBallAndGoal(Ball* ball, Goal* goal) {
    if (!ball || !goal) {
        return false;
    }

    Vector3 ballPos = ball->GetPosition();
    Vector3 goalPos = goal->GetPosition();

    float distance = myMath::Distance(ballPos, goalPos);

    float collisionRadius = BALL_COLLISION_RADIUS + GOAL_COLLISION_RADIUS;
    return distance <= collisionRadius;
}

void GameLogicManager::HandleBallCollisions() {
    if (!balls_ || balls_->empty()) {
        return;
    }

    for (size_t i = 0; i < balls_->size(); i++) {
        Ball* ball1 = (*balls_)[i].get();

        if (!ball1 || !ball1->IsActive() || ball1->IsExploded()) {
            continue;
        }

        for (size_t j = i + 1; j < balls_->size(); j++) {
            Ball* ball2 = (*balls_)[j].get();

            if (!ball2 || !ball2->IsActive() || ball2->IsExploded()) {
                continue;
            }

            if (CheckBallBallCollision(ball1, ball2)) {
                ResolveBallCollision(ball1, ball2);
            }
        }
    }
}

bool GameLogicManager::CheckBallBallCollision(Ball* ball1, Ball* ball2) {
    if (!ball1 || !ball2) {
        return false;
    }

    Vector3 pos1 = ball1->GetPosition();
    Vector3 pos2 = ball2->GetPosition();

    float distance = myMath::Distance(pos1, pos2);

    float collisionDistance = BALL_COLLISION_RADIUS * 2.0f;
    return distance <= collisionDistance;
}

void GameLogicManager::ResolveBallCollision(Ball* ball1, Ball* ball2) {
    if (!ball1 || !ball2) {
        return;
    }

    Vector3 pos1 = ball1->GetPosition();
    Vector3 pos2 = ball2->GetPosition();
    Vector3 vel1 = ball1->GetVelocity();
    Vector3 vel2 = ball2->GetVelocity();

    Vector3 collisionNormal = myMath::Subtract(pos2, pos1);
    float distance = myMath::Length(collisionNormal);

    if (distance == 0.0f) {
        return;
    }

    collisionNormal = myMath::Multiply(1.0f / distance, collisionNormal);

    Vector3 relativeVelocity = myMath::Subtract(vel1, vel2);
    float velocityAlongNormal = Dot(relativeVelocity, collisionNormal);

    if (velocityAlongNormal > 0) {
        return;
    }

    const float BALL_RADIUS = 2.0f;
    const float MIN_DISTANCE = BALL_RADIUS * 2.0f;

    float restitution = 0.9f;

    float impulseScalar = -(1.0f + restitution) * velocityAlongNormal;
    impulseScalar *= 1.2f;

    Vector3 impulse1 = myMath::Multiply(-impulseScalar * 0.6f, collisionNormal);
    Vector3 impulse2 = myMath::Multiply(impulseScalar * 0.6f, collisionNormal);

    ball1->StartKnockback(
        impulse1,
        0.6f,
        50.0f,
        false
    );

    ball2->StartKnockback(
        impulse2,
        0.6f,
        50.0f,
        false
    );

    if (distance < MIN_DISTANCE && distance > 0.0f) {
        float overlap = MIN_DISTANCE - distance;
        Vector3 separation = myMath::Multiply(overlap * 0.5f, collisionNormal);

        ball1->SetPosition(myMath::Subtract(pos1, separation));
        ball2->SetPosition(myMath::Add(pos2, separation));
    }
}

bool GameLogicManager::IsMouseOverBall(Ball* ball, const Vector2& mousePos) {
    if (!ball) {
        return false;
    }

    Vector3 worldPos = ball->GetPosition();
    Vector3 screenPos = WorldToScreen(worldPos);

    float distance = std::sqrt(
        std::pow(mousePos.x - screenPos.x, 2) +
        std::pow(mousePos.y - screenPos.y, 2)
    );

    return distance <= SCREEN_BALL_RADIUS;
}

Vector3 GameLogicManager::WorldToScreen(const Vector3& worldPos) {
    if (!camera_) {
        return { 0.0f, 0.0f, 0.0f };
    }

    camera_->UpdateMatrix();

    const Matrix4x4& viewMatrix = camera_->matView;
    const Matrix4x4& projectionMatrix = camera_->matProjection;
    Matrix4x4 viewProjectionMatrix = myMath::Multiply(viewMatrix, projectionMatrix);

    Vector4 worldPos4 = {
        worldPos.x,
        worldPos.y,
        worldPos.z,
        1.0f
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

    float screenX = (clipPos.x + 1.0f) * 0.5f * 1280.0f;
    float screenY = (1.0f - (clipPos.y + 1.0f) * 0.5f) * 720.0f;

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