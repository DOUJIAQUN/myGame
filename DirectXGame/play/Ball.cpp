#include "Ball.h"
#include <cmath>

using namespace KamataEngine;

Ball::~Ball() { 
    delete model_; 
    if (explosionRangeSprite_) {
	    delete explosionRangeSprite_;
    }
}

void Ball::Initialize(Camera* camera) {
	camera_ = camera;
	worldTransform_.Initialize();
	model_ = Model::CreateFromOBJ("Player", true);
	input_ = Input::GetInstance();


	worldTransform_.translation_ = {-30, 0, 0};
	

	worldTransform_.scale_ = {2, 2, 2};
	initialScale_ = worldTransform_.scale_;


	isExploded_ = false;
	isActive_ = true;
	velocity_ = {0, 0, 0};
	isMouseOver_ = false;

	// 旋转初始化
	rotation_ = 0.0f;
	initialRotation_ = rotation_;
	rotationSpeed_ = 0.3f; // 1弧度/秒，大约57度/秒

	
	// 加载爆炸范围图片
	explosionRangeTextureHandle_ = TextureManager::Load("ui/explosionRange.png");

	// 创建爆炸范围精灵（初始位置设为0，0，在Update中更新位置）
	explosionRangeSprite_ = Sprite::Create(explosionRangeTextureHandle_, { 0, 0 });

	// 设置爆炸范围大小（根据爆炸半径11.0f调整）
	// 假设爆炸半径11.0f对应屏幕上的某个尺寸，需要根据实际调整
	const float explosionRangeSize = 400.0f; // 示例值，需要根据实际调整
	if (explosionRangeSprite_) {
		explosionRangeSprite_->SetSize({ explosionRangeSize, explosionRangeSize });
		// 设置锚点为图片中心 (0.5, 0.5)
		explosionRangeSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	}
}

//设置初始位置
void Ball::SetInitialPosition(const KamataEngine::Vector3& position) {
	initialPosition_ = position;
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();
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
	isMouseOver_ = false; // 鼠标悬停状态重置
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

	// 更新旋转角度（无论是否悬停都更新，这样切换时旋转更平滑）
	rotation_ += rotationSpeed_ * (1.0f / 60.0f); // 假设60帧

	// 保持旋转角度在 0-2π 范围内
	if (rotation_ > 2 * 3.14159265f) {
		rotation_ -= 2 * 3.14159265f;
	}




	worldTransform_.UpdateMatrix();
}

// 新增方法：使用屏幕坐标更新爆炸范围位置
void Ball::UpdateExplosionRangePosition(const KamataEngine::Vector3& screenPos) {
	if (isMouseOver_ && isActive_ && !isExploded_ && explosionRangeSprite_ != nullptr) {
		// 由于设置了中心锚点，现在可以直接使用屏幕坐标作为位置
		// 锚点为(0.5, 0.5)时，Sprite的位置就是图片的中心点
		explosionRangeSprite_->SetPosition({ screenPos.x, screenPos.y });

		// 设置旋转
		explosionRangeSprite_->SetRotation(rotation_);
	}
}

void Ball::Draw() {
	if (isActive_) {
		model_->Draw(worldTransform_, *camera_);
	}

}

void Ball::DrawExplosionRange() {
	// 如果鼠标悬停且球体活跃且未爆炸，绘制爆炸范围
	if (isMouseOver_ && isActive_ && !isExploded_) {
		if (explosionRangeSprite_ != nullptr) {
			explosionRangeSprite_->Draw();
		}
	}
}

void Ball::Reset() {
	// 重置位置到各自独特的初始位置
	worldTransform_.translation_ = initialPosition_;

	// 重置状态
	isExploded_ = false;
	isActive_ = true;
	isMouseOver_ = false;

	// 重置速度
	velocity_ = { 0, 0, 0 };

	// 重置旋转
	rotation_ = 0.0f;

	// 更新变换矩阵
	worldTransform_.UpdateMatrix();
}


