#include "Goal.h"
#include <cmath>
#include <memory>

#include "../DebugLogger.h"
#include "../play/GoalMoveStrategies.h"

using namespace KamataEngine;

namespace {
	constexpr float kFrameDeltaTime = 1.0f / 60.0f;
	constexpr float kDefaultPositionX = -30.0f;
	constexpr float kDefaultPositionY = 0.0f;
	constexpr float kDefaultPositionZ = 0.0f;
	constexpr float kDefaultScale = 2.0f;
}

Goal::~Goal() = default;

void Goal::Initialize(Camera* camera) {
	camera_ = camera;
	worldTransform_.Initialize();
	model_.reset(Model::CreateFromOBJ("cube", true));
	input_ = Input::GetInstance();
	worldTransform_.translation_ = { kDefaultPositionX, kDefaultPositionY, kDefaultPositionZ };
	worldTransform_.scale_ = { kDefaultScale, kDefaultScale, kDefaultScale };

	// 初始化移动相关变量
	movementConfig_ = GoalMovementConfig(); // 默认配置（不移动）
	moveTimer_ = 0.0f;
	initialPosition_ = worldTransform_.translation_;

	// 默认策略：不移动
	moveStrategy_ = std::make_unique<NoMoveStrategy>();
}

// 设置移动配置
void Goal::SetMovementConfig(const GoalMovementConfig& config) {
	movementConfig_ = config;
	initialPosition_ = worldTransform_.translation_; // 更新初始位置
	moveTimer_ = 0.0f;                               // 重新计时（可选，但更符合“从初始位置开始移动”）
	SetMoveStrategyByConfig();
}

// Strategy 选择（只负责“选择”，具体移动算法放在策略类里）
void Goal::SetMoveStrategyByConfig() {
	if (!movementConfig_.shouldMove) {
		moveStrategy_ = std::make_unique<NoMoveStrategy>();
		return;
	}

	switch (movementConfig_.direction) {
	case MoveDirection::Horizontal:
		moveStrategy_ = std::make_unique<HorizontalMoveStrategy>();
		break;
	case MoveDirection::Vertical:
		moveStrategy_ = std::make_unique<VerticalMoveStrategy>();
		break;
	case MoveDirection::Circular:
		moveStrategy_ = std::make_unique<CircularMoveStrategy>();
		break;
	default:
		moveStrategy_ = std::make_unique<NoMoveStrategy>();
		break;
	}
}

// SetPosition 方法的实现
void Goal::SetPosition(const KamataEngine::Vector3& position) {
	worldTransform_.translation_ = position;
	initialPosition_ = position; // 同时设置初始位置
	worldTransform_.UpdateMatrix();
}

void Goal::Update() {
	// 如果需要移动且策略存在，更新位置
	if (movementConfig_.shouldMove && moveStrategy_) {
		moveTimer_ += kFrameDeltaTime; // 假设60帧
		moveStrategy_->Apply(worldTransform_, initialPosition_, moveTimer_, movementConfig_);
	}

	worldTransform_.UpdateMatrix();
}

void Goal::Draw() {
	model_->Draw(worldTransform_, *camera_);
}
