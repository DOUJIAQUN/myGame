#include "Ball.h"
#include <cmath>


using namespace KamataEngine;

Ball::~Ball() { 
    delete model_; 
    if (explosionRangeSprite_) {
	    delete explosionRangeSprite_;
    }
	if (explosionSprite_) {
		delete explosionSprite_;
	}
	CleanupTrail();
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

	// 初始化爆炸特效
	// 加载爆炸特效图片
	explosionTextureHandles_.push_back(TextureManager::Load("effect/boom1.png"));
	explosionTextureHandles_.push_back(TextureManager::Load("effect/boom2.png"));
	explosionTextureHandles_.push_back(TextureManager::Load("effect/boom3.png"));
	explosionTextureHandles_.push_back(TextureManager::Load("effect/boom4.png"));

	// 创建爆炸特效精灵（初始位置设为0，0）
	explosionSprite_ = Sprite::Create(explosionTextureHandles_[0], { 0, 0 });
	if (explosionSprite_) {
		const float explosionSize = 300.0f; // 爆炸特效大小
		explosionSprite_->SetSize({ explosionSize, explosionSize });
		explosionSprite_->SetAnchorPoint({ 0.5f, 0.5f });
		explosionSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // 初始完全透明
	}

	isExplosionAnimPlaying_ = false;
	currentExplosionFrame_ = 0;
	explosionAnimTimer_ = 0.0f;

	// 初始化击退锁定状态
	isKnockbackLocked_ = false;
	knockbackLockTimer_ = 0.0f;

	// 初始化拖尾特效
	trailPoints_.clear();
	trailSpawnTimer_ = 0.0f;
	trailTextureHandle_ = TextureManager::Load("effect/trail.png"); // 加载拖尾纹理，如果没有可以用爆炸范围纹理代替
	if (trailTextureHandle_ == 0) {
		trailTextureHandle_ = explosionRangeTextureHandle_; // 如果没有拖尾纹理，使用爆炸范围纹理
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
	// 如果已经爆炸或是不活跃状态，不重复爆炸
	if (isExploded_ || !isActive_) return;


	isExploded_ = true;
	isActive_ = false; // 球体消失
	isMouseOver_ = false; // 鼠标悬停状态重置


	// 清空拖尾
	CleanupTrail();

	// 开始播放爆炸动画
	isExplosionAnimPlaying_ = true;
	currentExplosionFrame_ = 0;
	explosionAnimTimer_ = 0.0f;

	// 设置爆炸特效位置为球体位置
	if (explosionSprite_) {
		Vector3 screenPos = WorldToScreen(worldTransform_.translation_);
		explosionSprite_->SetPosition({ screenPos.x, screenPos.y });
		explosionSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f }); // 设置为不透明
	}
}

void Ball::ApplyExplosionForce(const KamataEngine::Vector3& force) {
	velocity_ = myMath::Add(velocity_, force);

	// 应用爆炸力时进入击退锁定状态
	isKnockbackLocked_ = true;
	knockbackLockTimer_ = 0.0f;
}


void Ball::Update() {

	// 更新爆炸动画
	if (isExplosionAnimPlaying_) {
		explosionAnimTimer_ += 1.0f / 60.0f; // 假设60帧

		// 检查是否需要切换到下一帧
		if (explosionAnimTimer_ >= explosionFrameDuration_) {
			explosionAnimTimer_ = 0.0f;
			currentExplosionFrame_++;

			// 更新爆炸特效纹理
			if (explosionSprite_ && currentExplosionFrame_ < explosionTotalFrames_) {
				explosionSprite_->SetTextureHandle(explosionTextureHandles_[currentExplosionFrame_]);
			}

			// 检查动画是否结束
			if (currentExplosionFrame_ >= explosionTotalFrames_) {
				isExplosionAnimPlaying_ = false;
				if (explosionSprite_) {
					explosionSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // 动画结束后设置为透明
				}
			}
		}
	}

	if (!isActive_) {
		return;
	}
	// 更新击退锁定状态
	UpdateKnockbackLock();

	// 更新拖尾特效
	UpdateTrail();


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


// 更新击退锁定状态
void Ball::UpdateKnockbackLock() {
	if (isKnockbackLocked_) {
		knockbackLockTimer_ += 1.0f / 60.0f;

		// 检查锁定时间是否结束
		if (knockbackLockTimer_ >= knockbackLockDuration_) {
			isKnockbackLocked_ = false;
			knockbackLockTimer_ = 0.0f;
		}

		// 如果速度很小，也提前结束锁定
		if (myMath::Length(velocity_) < 0.1f) {
			isKnockbackLocked_ = false;
			knockbackLockTimer_ = 0.0f;
		}
	}
}

// 更新拖尾特效
void Ball::UpdateTrail() {
	// 只有在移动且活跃状态下才生成拖尾
	if (isActive_ && myMath::Length(velocity_) > 0.1f) {
		trailSpawnTimer_ += 1.0f / 60.0f;

		// 达到生成间隔时添加新的拖尾点
		if (trailSpawnTimer_ >= trailSpawnInterval_) {
			trailSpawnTimer_ = 0.0f;
			AddTrailPoint();
		}
		// 更新所有拖尾点的生命周期
		for (auto it = trailPoints_.begin(); it != trailPoints_.end(); ) {
			it->lifetime += 1.0f / 60.0f;
			it->alpha = 1.0f - (it->lifetime / it->maxLifetime);

			// 如果拖尾点生命周期结束，移除它
			if (it->lifetime >= it->maxLifetime) {
				if (it->sprite) {
					delete it->sprite;
					it->sprite = nullptr;
				}
				it = trailPoints_.erase(it);
			}
			else {
				++it;
			}
		}

		// 限制拖尾点数量
		if (trailPoints_.size() > maxTrailPoints_) {
			// 移除最旧的拖尾点
			auto& oldest = trailPoints_.front();
			if (oldest.sprite) {
				delete oldest.sprite;
				oldest.sprite = nullptr;
			}
			trailPoints_.erase(trailPoints_.begin());
		}
	}

	// 更新所有拖尾点的生命周期
	for (auto it = trailPoints_.begin(); it != trailPoints_.end(); ) {
		it->lifetime += 1.0f / 60.0f;
		it->alpha = 1.0f - (it->lifetime / it->maxLifetime);

		// 如果拖尾点生命周期结束，移除它
		if (it->lifetime >= it->maxLifetime) {
			it = trailPoints_.erase(it);
		}
		else {
			++it;
		}
	}

	// 限制拖尾点数量
	if (trailPoints_.size() > maxTrailPoints_) {
		trailPoints_.erase(trailPoints_.begin());
	}
}

// 添加拖尾点
void Ball::AddTrailPoint() {
	TrailPoint newPoint;
	newPoint.position = worldTransform_.translation_;
	newPoint.lifetime = 0.0f;
	newPoint.maxLifetime = 0.5f; // 拖尾点存在0.5秒
	newPoint.alpha = 1.0f;

	// 为每个拖尾点创建独立的精灵
	Vector3 screenPos = WorldToScreen(newPoint.position);
	newPoint.sprite = Sprite::Create(trailTextureHandle_, { screenPos.x, screenPos.y });
	if (newPoint.sprite) {
		newPoint.sprite->SetSize({ trailSize_, trailSize_ });
		newPoint.sprite->SetAnchorPoint({ 0.5f, 0.5f });
		newPoint.sprite->SetColor({ 1.0f, 1.0f, 1.0f, newPoint.alpha });
	}

	trailPoints_.push_back(newPoint);
}

// 绘制拖尾特效
void Ball::DrawTrail() {
	for (const auto& trailPoint : trailPoints_) {
		if (trailPoint.sprite) {
			// 更新位置（因为相机可能移动了）
			Vector3 screenPos = WorldToScreen(trailPoint.position);
			trailPoint.sprite->SetPosition({ screenPos.x, screenPos.y });

			// 更新透明度
			trailPoint.sprite->SetColor({ 1.0f, 1.0f, 1.0f, trailPoint.alpha });

			// 绘制
			trailPoint.sprite->Draw();
		}
	}
}

// 清理拖尾资源
void Ball::CleanupTrail() {
	for (auto& trailPoint : trailPoints_) {
		if (trailPoint.sprite) {
			delete trailPoint.sprite;
			trailPoint.sprite = nullptr;
		}
	}
	trailPoints_.clear();
}

// 世界坐标转屏幕坐标的辅助函数
KamataEngine::Vector3 Ball::WorldToScreen(const KamataEngine::Vector3& worldPos) {
	// 使用相机的视图投影矩阵进行坐标转换
	const Matrix4x4& viewMatrix = camera_->matView;
	const Matrix4x4& projectionMatrix = camera_->matProjection;
	Matrix4x4 viewProjectionMatrix = myMath::Multiply(viewMatrix, projectionMatrix);

	// 变换到齐次裁剪空间
	Vector4 worldPos4 = { worldPos.x, worldPos.y, worldPos.z, 1.0f };
	Vector4 clipPos = myMath::Transform(worldPos4, viewProjectionMatrix);

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

//使用屏幕坐标更新爆炸范围位置
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
	if (isMouseOver_ && isActive_ && !isExploded_ && explosionRangeSprite_ != nullptr) {
		
			explosionRangeSprite_->Draw();
		
	}

	// 绘制爆炸特效（如果正在播放）
	if (isExplosionAnimPlaying_ && explosionSprite_ != nullptr) {
		explosionSprite_->Draw();
	}
	// 绘制拖尾特效
	DrawTrail();
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

	// 重置爆炸动画
	isExplosionAnimPlaying_ = false;
	currentExplosionFrame_ = 0;
	explosionAnimTimer_ = 0.0f;
	if (explosionSprite_) {
		explosionSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // 重置为透明
	}

	// 重置击退锁定状态
	isKnockbackLocked_ = false;
	knockbackLockTimer_ = 0.0f;

	// 清空拖尾点
	CleanupTrail();
	trailSpawnTimer_ = 0.0f;

	// 更新变换矩阵
	worldTransform_.UpdateMatrix();
}
