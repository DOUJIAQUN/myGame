#include "Ball.h"
#include <cmath>

using namespace KamataEngine;

namespace {
	constexpr float kFrameDeltaTime = 1.0f / 60.0f;
	constexpr float kDefaultPositionX = -30.0f;
	constexpr float kDefaultPositionY = 0.0f;
	constexpr float kDefaultPositionZ = 0.0f;
	constexpr float kDefaultScale = 2.0f;
	constexpr float kDefaultRotationSpeed = 0.3f;
	constexpr float kExplosionRangeSpriteSize = 400.0f;
	constexpr float kExplosionSpriteSize = 300.0f;
	constexpr float kSpriteAnchorCenter = 0.5f;
	constexpr float kColorMin = 0.0f;
	constexpr float kColorMax = 1.0f;
	constexpr float kDefaultKnockbackDuration = 0.5f;
	constexpr float kExplosionForceDuration = 0.4f;
	constexpr float kDefaultForceMultiplier = 1.0f;
	constexpr float kVelocityStopThreshold = 0.01f;
	constexpr float kKnockbackStopThreshold = 0.1f;
	constexpr float kPi = 3.14159265f;
	constexpr float kFullRotation = 2.0f * kPi;
	constexpr float kExplosionSlowDownBase = 0.9f;
	constexpr float kExplosionSlowDownRange = 0.2f;
	constexpr float kCollisionSlowDownBase = 0.95f;
	constexpr float kCollisionSlowDownRange = 0.15f;
	constexpr float kNormalSlowDown = 0.9f;
	constexpr float kDefaultTrailLifetime = 0.5f;
	constexpr float kExplosionTrailInterval = 0.02f;
	constexpr int kExplosionTrailMaxPoints = 20;
	constexpr float kExplosionTrailSize = 80.0f;
	constexpr float kExplosionTrailLifetime = 0.7f;
	constexpr float kCollisionTrailInterval = 0.03f;
	constexpr int kCollisionTrailMaxPoints = 15;
	constexpr float kCollisionTrailSize = 70.0f;
	constexpr float kCollisionTrailLifetime = 0.6f;
	constexpr float kAdditionalTrailSpeedThreshold = 1.5f;
	constexpr float kAdditionalTrailSizeRate = 0.8f;
	constexpr float kTrailExplosionColorR = 1.0f;
	constexpr float kTrailExplosionColorG = 0.7f;
	constexpr float kTrailExplosionColorB = 0.3f;
	constexpr float kTrailCollisionColorR = 0.8f;
	constexpr float kTrailCollisionColorG = 0.9f;
	constexpr float kTrailCollisionColorB = 1.0f;
	constexpr float kNdcOffset = 1.0f;
	constexpr float kNdcToScreenScale = 0.5f;
	constexpr float kScreenWidth = 1280.0f;
	constexpr float kScreenHeight = 720.0f;
}

namespace MyEngine {

Ball::~Ball() = default;

void Ball::Initialize(Camera* camera) {
	// Model と Sprite を生成し、クリック前の初期位置・速度・表示状態をまとめて初期化する。
	camera_ = camera;
	worldTransform_.Initialize();
	model_.reset(Model::CreateFromOBJ("Player", true));
	input_ = Input::GetInstance();

	worldTransform_.translation_ = { kDefaultPositionX, kDefaultPositionY, kDefaultPositionZ };
	worldTransform_.scale_ = { kDefaultScale, kDefaultScale, kDefaultScale };
	initialScale_ = worldTransform_.scale_;

	isExploded_ = false;
	isActive_ = true;
	velocity_ = { 0, 0, 0 };
	isMouseOver_ = false;

	// 旋转初始化
	rotation_ = 0.0f;
	initialRotation_ = rotation_;
	rotationSpeed_ = kDefaultRotationSpeed;

	// 加载爆炸范围图片
	explosionRangeTextureHandle_ = TextureManager::Load("ui/explosionRange.png");

	// 创建爆炸范围精灵（初始位置设为0，0，在Update中更新位置）
	explosionRangeSprite_.reset(Sprite::Create(explosionRangeTextureHandle_, { kColorMin, kColorMin }));

	// 设置爆炸范围大小（根据爆炸半径11.0f调整）
	if (explosionRangeSprite_) {
		explosionRangeSprite_->SetSize({ kExplosionRangeSpriteSize, kExplosionRangeSpriteSize });
		// 设置锚点为图片中心 (0.5, 0.5)
		explosionRangeSprite_->SetAnchorPoint({ kSpriteAnchorCenter, kSpriteAnchorCenter });
	}

	// 初始化爆炸特效
	explosionTextureHandles_.push_back(TextureManager::Load("effect/boom1.png"));
	explosionTextureHandles_.push_back(TextureManager::Load("effect/boom2.png"));
	explosionTextureHandles_.push_back(TextureManager::Load("effect/boom3.png"));
	explosionTextureHandles_.push_back(TextureManager::Load("effect/boom4.png"));

	// 创建爆炸特效精灵（初始位置设为0，0）
	explosionSprite_.reset(Sprite::Create(explosionTextureHandles_[0], { kColorMin, kColorMin }));
	if (explosionSprite_) {
		explosionSprite_->SetSize({ kExplosionSpriteSize, kExplosionSpriteSize });
		explosionSprite_->SetAnchorPoint({ kSpriteAnchorCenter, kSpriteAnchorCenter });
		explosionSprite_->SetColor({ kColorMax, kColorMax, kColorMax, kColorMin }); // 初始完全透明
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

	// 初始化击飞状态
	isKnockedBack_ = false;
	knockbackTimer_ = 0.0f;
	knockbackDuration_ = kDefaultKnockbackDuration;
	knockbackForce_ = { 0, 0, 0 };
	isExplosionKnockback_ = false;
}

// 设置初始位置
void Ball::SetInitialPosition(const KamataEngine::Vector3& position) {
	initialPosition_ = position;
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();
}

// SetPosition 方法的实现
void Ball::SetPosition(const KamataEngine::Vector3& position) {
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();
}

void Ball::Explode() {
	// 爆発済みの Ball は再度クリックできないため、状態フラグを先に切り替える。
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
		explosionSprite_->SetColor({ kColorMax, kColorMax, kColorMax, kColorMax }); // 设置为不透明
	}
}

void Ball::ApplyExplosionForce(const KamataEngine::Vector3& force) {
	// 使用统一的击飞方法，爆炸击飞使用特定参数
	StartKnockback(
		force,
		kExplosionForceDuration,
		kDefaultForceMultiplier,
		true
	);
}

// 统一的击飞方法
void Ball::StartKnockback(const KamataEngine::Vector3& force,
	float duration,
	float forceMultiplier,
	bool isExplosionKnockback) {
	if (!isActive_ || isExploded_) return;

	// 计算增强后的击飞力
	KamataEngine::Vector3 enhancedForce = myMath::Multiply(forceMultiplier, force);

	// 设置击飞状态
	isKnockedBack_ = true;
	knockbackTimer_ = 0.0f;
	knockbackDuration_ = duration;
	knockbackForce_ = enhancedForce;
	isExplosionKnockback_ = isExplosionKnockback;

	// 应用初始速度
	velocity_ = myMath::Add(velocity_, enhancedForce);

	// 清空当前拖尾，重新开始
	CleanupTrail();
	trailSpawnTimer_ = 0.0f;

	// 设置击退锁定
	if (isExplosionKnockback) {
		// 爆炸击飞使用原有的击退锁定逻辑
		isKnockbackLocked_ = true;
		knockbackLockTimer_ = 0.0f;
	}
}

void Ball::Update() {
	// Ball の状態に応じて、爆発アニメーション、ノックバック、トレイル、回転を順番に更新する。
	// 更新爆炸动画
	if (isExplosionAnimPlaying_) {
		explosionAnimTimer_ += kFrameDeltaTime; // 假设60帧

		// 检查是否需要切换到下一帧
		if (explosionAnimTimer_ >= kExplosionFrameDuration) {
			explosionAnimTimer_ = 0.0f;
			currentExplosionFrame_++;

			// 更新爆炸特效纹理
			if (explosionSprite_ && currentExplosionFrame_ < kExplosionTotalFrames) {
				explosionSprite_->SetTextureHandle(explosionTextureHandles_[currentExplosionFrame_]);
			}

			// 检查动画是否结束
			if (currentExplosionFrame_ >= kExplosionTotalFrames) {
				isExplosionAnimPlaying_ = false;
				if (explosionSprite_) {
					explosionSprite_->SetColor({ kColorMax, kColorMax, kColorMax, kColorMin }); // 动画结束后设置为透明
				}
			}
		}
	}

	if (!isActive_) {
		return;
	}

	// 更新击飞状态
	if (isKnockedBack_) {
		UpdateKnockback();
	}

	// 更新击退锁定状态
	UpdateKnockbackLock();

	// 更新拖尾特效
	UpdateTrail();

	// 如果受到力影响，更新位置
	if (myMath::Length(velocity_) > kVelocityStopThreshold) {
		// 使用缓动函数使速度逐渐减慢
		float slowDownFactor = CalculateSlowDownFactor();
		velocity_ = myMath::Multiply(slowDownFactor, velocity_);
		worldTransform_.translation_ = myMath::Add(worldTransform_.translation_, velocity_);
	}

	// 更新旋转角度（无论是否悬停都更新，这样切换时旋转更平滑）
	rotation_ += rotationSpeed_ * kFrameDeltaTime; // 假设60帧

	// 保持旋转角度在 0-2π 范围内
	if (rotation_ > kFullRotation) {
		rotation_ -= kFullRotation;
	}

	worldTransform_.UpdateMatrix();
}

// 计算减速因子
float Ball::CalculateSlowDownFactor() {
	// ノックバックの種類によって減速率を変え、爆発時と衝突時の手触りを分ける。
	if (isKnockedBack_) {
		float progress = knockbackTimer_ / knockbackDuration_;

		if (isExplosionKnockback_) {
			// 爆炸击飞：初始很快，然后快速减速
			return kExplosionSlowDownBase - (progress * kExplosionSlowDownRange); // 0.9 → 0.7
		}
		else {
			// 相撞击飞：相对平稳的减速
			return kCollisionSlowDownBase - (progress * kCollisionSlowDownRange); // 0.95 → 0.8
		}
	}
	else {
		// 普通移动：较快减速
		return kNormalSlowDown;
	}
}

// 更新击飞状态
void Ball::UpdateKnockback() {
	// ノックバック力を速度として位置に加算し、経過時間に応じて徐々に減速させる。
	if (!isKnockedBack_) return;

	knockbackTimer_ += kFrameDeltaTime;

	if (knockbackTimer_ >= knockbackDuration_) {
		// 击飞结束
		isKnockedBack_ = false;
		knockbackTimer_ = 0.0f;
		knockbackDuration_ = kDefaultKnockbackDuration;
		knockbackForce_ = { 0, 0, 0 };
		isExplosionKnockback_ = false;
	}
}

// 更新击退锁定状态
void Ball::UpdateKnockbackLock() {
	if (isKnockbackLocked_) {
		knockbackLockTimer_ += kFrameDeltaTime;

		// 检查锁定时间是否结束
		if (knockbackLockTimer_ >= kKnockbackLockDuration) {
			isKnockbackLocked_ = false;
			knockbackLockTimer_ = 0.0f;
		}

		// 如果速度很小，也提前结束锁定
		if (myMath::Length(velocity_) < kKnockbackStopThreshold) {
			isKnockbackLocked_ = false;
			knockbackLockTimer_ = 0.0f;
		}
	}
}

// 更新拖尾特效
void Ball::UpdateTrail() {
	// 移動中だけ一定間隔でトレイル点を追加し、寿命を超えた点から順に削除する。
	// 决定使用哪种拖尾参数
	float currentSpawnInterval = kTrailSpawnInterval;
	int currentMaxPoints = kMaxTrailPoints;
	float currentTrailSize = kTrailSize;
	Vector4 trailColor = { kColorMax, kColorMax, kColorMax, kColorMax };
	float trailLifetime = kDefaultTrailLifetime;

	if (isKnockedBack_) {
		if (isExplosionKnockback_) {
			// 爆炸击飞配置
			currentSpawnInterval = kExplosionTrailInterval;
			currentMaxPoints = kExplosionTrailMaxPoints;
			currentTrailSize = kExplosionTrailSize;
			trailColor = { kTrailExplosionColorR, kTrailExplosionColorG, kTrailExplosionColorB, kColorMax }; // 橙黄色
			trailLifetime = kExplosionTrailLifetime;
		}
		else {
			// 相撞击飞配置
			currentSpawnInterval = kCollisionTrailInterval;
			currentMaxPoints = kCollisionTrailMaxPoints;
			currentTrailSize = kCollisionTrailSize;
			trailColor = { kTrailCollisionColorR, kTrailCollisionColorG, kTrailCollisionColorB, kColorMax }; // 淡蓝色
			trailLifetime = kCollisionTrailLifetime;
		}
	}

	// 只有在移动且活跃状态下才生成拖尾
	if (isActive_ && myMath::Length(velocity_) > kKnockbackStopThreshold) {
		trailSpawnTimer_ += kFrameDeltaTime;

		// 达到生成间隔时添加新的拖尾点
		if (trailSpawnTimer_ >= currentSpawnInterval) {
			trailSpawnTimer_ = 0.0f;

			// 根据配置添加拖尾点
			AddTrailPointWithConfig(currentTrailSize, trailColor, trailLifetime);

			// 爆炸击飞时生成更密集的拖尾
			if (isExplosionKnockback_ && myMath::Length(velocity_) > kAdditionalTrailSpeedThreshold) {
				AddTrailPointWithConfig(currentTrailSize * kAdditionalTrailSizeRate, trailColor, trailLifetime);
			}
		}

		// 更新所有拖尾点的生命周期
		for (auto it = trailPoints_.begin(); it != trailPoints_.end(); ) {
			it->lifetime += kFrameDeltaTime;
			it->alpha = kColorMax - (it->lifetime / it->maxLifetime);

			// 如果拖尾点生命周期结束，移除它
			if (it->lifetime >= it->maxLifetime) {
				it = trailPoints_.erase(it);
			}
			else {
				++it;
			}
		}

		// 限制拖尾点数量
		if (trailPoints_.size() > currentMaxPoints) {
			// 移除最旧的拖尾点
			trailPoints_.erase(trailPoints_.begin());
		}
	}

	// 更新所有拖尾点的生命周期（不活跃时的更新）
	for (auto it = trailPoints_.begin(); it != trailPoints_.end(); ) {
		it->lifetime += kFrameDeltaTime;
		it->alpha = kColorMax - (it->lifetime / it->maxLifetime);

		// 如果拖尾点生命周期结束，移除它
		if (it->lifetime >= it->maxLifetime) {
			it = trailPoints_.erase(it);
		}
		else {
			++it;
		}
	}

	// 限制拖尾点数量（不活跃时的限制）
	if (trailPoints_.size() > currentMaxPoints) {
		trailPoints_.erase(trailPoints_.begin());
	}
}

// 带配置的拖尾点添加方法
void Ball::AddTrailPointWithConfig(float size, const Vector4& color, float lifetime) {
	// 3D 座標を画面座標へ変換し、その位置にトレイル用 Sprite を生成する。
	TrailPoint newPoint;
	newPoint.position = worldTransform_.translation_;
	newPoint.lifetime = kColorMin;
	newPoint.maxLifetime = lifetime;
	newPoint.alpha = kColorMax;

	// 为每个拖尾点创建独立的精灵
	Vector3 screenPos = WorldToScreen(newPoint.position);
	newPoint.sprite.reset(Sprite::Create(trailTextureHandle_, { screenPos.x, screenPos.y }));
	if (newPoint.sprite) {
		newPoint.sprite->SetSize({ size, size });
		newPoint.sprite->SetAnchorPoint({ kSpriteAnchorCenter, kSpriteAnchorCenter });
		newPoint.sprite->SetColor({ color.x, color.y, color.z, newPoint.alpha });
	}

	trailPoints_.push_back(std::move(newPoint));
}

// 添加拖尾点
void Ball::AddTrailPoint(float size) {
	AddTrailPointWithConfig(size, { kColorMax, kColorMax, kColorMax, kColorMax }, kDefaultTrailLifetime);
}

// 绘制拖尾特效
void Ball::DrawTrail() {
	for (const auto& trailPoint : trailPoints_) {
		if (trailPoint.sprite) {
			// 更新位置（因为相机可能移动了）
			Vector3 screenPos = WorldToScreen(trailPoint.position);
			trailPoint.sprite->SetPosition({ screenPos.x, screenPos.y });

			// 更新透明度
			trailPoint.sprite->SetColor({
				trailPoint.sprite->GetColor().x,
				trailPoint.sprite->GetColor().y,
				trailPoint.sprite->GetColor().z,
				trailPoint.alpha
				});

			// 绘制
			trailPoint.sprite->Draw();
		}
	}
}

// 清理拖尾资源
void Ball::CleanupTrail() {
	trailPoints_.clear();
}

// 世界坐标转屏幕坐标的辅助函数
KamataEngine::Vector3 Ball::WorldToScreen(const KamataEngine::Vector3& worldPos) {
	// Camera の view-projection 行列でクリップ座標へ変換し、NDC から画面座標へ写像する。
	// 使用相机的视图投影矩阵进行坐标转换
	const Matrix4x4& viewMatrix = camera_->matView;
	const Matrix4x4& projectionMatrix = camera_->matProjection;
	Matrix4x4 viewProjectionMatrix = myMath::Multiply(viewMatrix, projectionMatrix);

	// 变换到齐次裁剪空间
	Vector4 worldPos4 = { worldPos.x, worldPos.y, worldPos.z, kColorMax };
	Vector4 clipPos = myMath::Transform(worldPos4, viewProjectionMatrix);

	// 透视除法
	if (clipPos.w != kColorMin) {
		clipPos.x /= clipPos.w;
		clipPos.y /= clipPos.w;
		clipPos.z /= clipPos.w;
	}

	// 转换到屏幕坐标
	// 假设屏幕分辨率为1280x720
	float screenX = (clipPos.x + kNdcOffset) * kNdcToScreenScale * kScreenWidth;
	float screenY = (kNdcOffset - (clipPos.y + kNdcOffset) * kNdcToScreenScale) * kScreenHeight;

	return { screenX, screenY, clipPos.z };
}

// 使用屏幕坐标更新爆炸范围位置
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
		explosionSprite_->SetColor({ kColorMax, kColorMax, kColorMax, kColorMin }); // 重置为透明
	}

	// 重置击退锁定状态
	isKnockbackLocked_ = false;
	knockbackLockTimer_ = 0.0f;

	// 重置击飞状态
	isKnockedBack_ = false;
	knockbackTimer_ = 0.0f;
	knockbackDuration_ = kDefaultKnockbackDuration;
	knockbackForce_ = { 0, 0, 0 };
	isExplosionKnockback_ = false;

	// 清空拖尾点
	CleanupTrail();
	trailSpawnTimer_ = 0.0f;

	// 更新变换矩阵
	worldTransform_.UpdateMatrix();
}

void Ball::OnEnterGoal(Goal* goal) {
	// 同じ Ball が Goal 内に居続けても重複カウントしないよう、接触開始時だけ呼ばれる。
	// 如果球已经不活跃或已爆炸，不再重复处理
	if (!isActive_ || isExploded_) return;

	// 通知目标增加计数
	goal->IncrementCount();

	// 球体自身变为非活跃（消失）
	isActive_ = false;
	isMouseOver_ = false;

	// 清理拖尾效果
	CleanupTrail();
}

} // namespace MyEngine
