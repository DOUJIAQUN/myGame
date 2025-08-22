#include "Ball.h"
#include <cmath>

using namespace KamataEngine;

Ball::~Ball() { delete model_; }

void Ball::Initialize(Camera* camera) {
	camera_ = camera;
	worldTransform_.Initialize();
	model_ = Model::CreateFromOBJ("Player", true);
	input_ = Input::GetInstance();
	worldTransform_.translation_ = {-30, 0, 0};
	worldTransform_.scale_ = {2, 2, 2};

}

// SetPosition 方法的实现
void Ball::SetPosition(const KamataEngine::Vector3& position) {
	worldTransform_.translation_ = position;
	// 如果需要立即更新矩阵，可以调用 UpdateMatrix
	worldTransform_.UpdateMatrix();
}

void Ball::Update() {
	Vector3 move = {0, 0, 0};
	

	worldTransform_.translation_ += move;

	worldTransform_.UpdateMatrix();
}

void Ball::Draw() { model_->Draw(worldTransform_, *camera_); }