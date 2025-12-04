#include "Goal.h"
#include <cmath>
#include "../DebugLogger.h" 
using namespace KamataEngine;

Goal::~Goal() { delete model_; }

void Goal::Initialize(Camera* camera) {
	camera_ = camera;
	worldTransform_.Initialize();
	model_ = Model::CreateFromOBJ("cube", true);
	input_ = Input::GetInstance();
	worldTransform_.translation_ = {-30, 0, 0};
	worldTransform_.scale_ = {2, 2, 2};

	// 初始化移动相关变量
	movementConfig_ = GoalMovementConfig(); // 默认配置（不移动）
	moveTimer_ = 0.0f;
	initialPosition_ = worldTransform_.translation_;
}

// 设置移动配置
void Goal::SetMovementConfig(const GoalMovementConfig& config) {
	movementConfig_ = config;
	initialPosition_ = worldTransform_.translation_; // 更新初始位置
}

// SetPosition 方法的实现
void Goal::SetPosition(const KamataEngine::Vector3& position) {
	worldTransform_.translation_ = position;
	initialPosition_ = position; // 同时设置初始位置
	// 如果需要立即更新矩阵，可以调用 UpdateMatrix
	worldTransform_.UpdateMatrix();
}





void Goal::Update() {

	// 如果需要移动，更新位置
	if (movementConfig_.shouldMove) {



		moveTimer_ += 1.0f / 60.0f; // 假设60帧

		// 根据移动方向计算移动距离（使用正弦波实现平滑的来回移动）
		float moveDistance = sin(moveTimer_ * movementConfig_.moveSpeed) * movementConfig_.moveRange;

		// 根据方向更新位置
		switch (movementConfig_.direction) {
		case MoveDirection::Horizontal:
			// 水平移动（左右）
			worldTransform_.translation_.x = initialPosition_.x + moveDistance;
			break;

		case MoveDirection::Vertical:
			// 垂直移动（上下）
			worldTransform_.translation_.y = initialPosition_.y + moveDistance;
			break;

		case MoveDirection::Circular:
			// 圆形移动（可选扩展）
			// 使用正弦和余弦实现圆形轨迹
			worldTransform_.translation_.x = initialPosition_.x + cos(moveTimer_ * movementConfig_.moveSpeed) * movementConfig_.moveRange;
			worldTransform_.translation_.y = initialPosition_.y + sin(moveTimer_ * movementConfig_.moveSpeed) * movementConfig_.moveRange;
			break;
		}
	}

	worldTransform_.UpdateMatrix();
}

void Goal::Draw() {

		model_->Draw(worldTransform_, *camera_);
	
}