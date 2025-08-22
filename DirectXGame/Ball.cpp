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
	isExploded_ = false;
	isActive_ = true;
	velocity_ = {0, 0, 0};
}

// SetPosition 方法的实现
void Ball::SetPosition(const KamataEngine::Vector3& position) {
	worldTransform_.translation_ = position;
	// 如果需要立即更新矩阵，可以调用 UpdateMatrix
	worldTransform_.UpdateMatrix();
}


void Ball::Explode() {
	isExploded_ = true;
	isActive_ = false; // 球体消失
}

void Ball::ApplyExplosionForce(const KamataEngine::Vector3& force) { velocity_ = myMath::Add(velocity_, force); }


void Ball::Update() {

 

	// 如果受到爆炸力影响，更新位置
	// 使用 myMath::Length 而不是全局的 Length 函数
	if (myMath::Length(velocity_) > 0.01f) {
		worldTransform_.translation_ = myMath::Add(worldTransform_.translation_, velocity_);
		// 模拟阻力，逐渐减小速度
		velocity_ = myMath::Multiply(0.9f, velocity_);
	}

	worldTransform_.UpdateMatrix();
}

void Ball::Draw() {
	if (isActive_) {
		model_->Draw(worldTransform_, *camera_);
	}
}