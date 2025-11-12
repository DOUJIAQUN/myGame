#include "Goal.h"
#include <cmath>

using namespace KamataEngine;

Goal::~Goal() { delete model_; }

void Goal::Initialize(Camera* camera) {
	camera_ = camera;
	worldTransform_.Initialize();
	model_ = Model::CreateFromOBJ("cube", true);
	input_ = Input::GetInstance();
	worldTransform_.translation_ = {-30, 0, 0};
	worldTransform_.scale_ = {2, 2, 2};
	
}

// SetPosition 方法的实现
void Goal::SetPosition(const KamataEngine::Vector3& position) {
	worldTransform_.translation_ = position;
	// 如果需要立即更新矩阵，可以调用 UpdateMatrix
	worldTransform_.UpdateMatrix();
}





void Goal::Update() {



	worldTransform_.UpdateMatrix();
}

void Goal::Draw() {

		model_->Draw(worldTransform_, *camera_);
	
}