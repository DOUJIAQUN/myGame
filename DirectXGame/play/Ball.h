#pragma once
#include "KamataEngine.h"
#include <functional>
#include "../math/myMath.h"

class Ball

{
public:
	~Ball();
	void Initialize(KamataEngine::Camera* camera);
	void Update();
	void Draw();


	KamataEngine::Vector3 GetPosition() const { return worldTransform_.translation_; }

	//接收屏幕坐标来更新爆炸范围位置
	void UpdateExplosionRangePosition(const KamataEngine::Vector3& screenPos);


	  // 爆炸相关方法
	void Explode();                                               // 触发爆炸
	bool IsExploded() const { return isExploded_; }               // 是否已爆炸
	bool IsActive() const { return isActive_; }                   // 是否活跃（未消失）
	void SetActive(bool active) {
		isActive_ = active;
		if (!active) {
		// 球体不活跃时，立即清空拖尾
		CleanupTrail();
	}
	}           // 设置活跃状态
	void ApplyExplosionForce(const KamataEngine::Vector3& force); // 应用爆炸力
	void DrawExplosionRange();

	// 鼠标悬停相关方法
	void SetMouseOver(bool isMouseOver) { isMouseOver_ = isMouseOver; }
	bool IsMouseOver() const { return isMouseOver_; }

	// 新增旋转相关方法
	void SetRotationSpeed(float speed) { rotationSpeed_ = speed; }
	float GetRotation() const { return rotation_; }

	// SetPosition方法，同时更新初始位置
	void SetInitialPosition(const KamataEngine::Vector3& position);
	void SetPosition(const KamataEngine::Vector3& position);

	//检查是否可被点击
	bool IsClickable() const { return isActive_ && !isExploded_ && !isKnockbackLocked_; }

	//设置击退锁定状态
	void SetKnockbackLocked(bool locked) { isKnockbackLocked_ = locked; }
	bool IsKnockbackLocked() const { return isKnockbackLocked_; }

	void Reset();
	
private:
	KamataEngine::Model* model_;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::ObjectColor objectColor;
	KamataEngine::Camera* camera_;
	KamataEngine::Input* input_ = nullptr;
	const float kSpeed = 0.2f;

	  // 爆炸相关变量
	bool isExploded_ = false;                    // 是否已爆炸
	bool isActive_ = true;                       // 是否活跃（未消失）
	KamataEngine::Vector3 velocity_ = {0, 0, 0}; // 速度向量
	

	// 鼠标悬停相关变量
	bool isMouseOver_ = false;                   // 鼠标是否悬停
	uint32_t explosionRangeTextureHandle_ = 0;   // 爆炸范围纹理句柄
	KamataEngine::Sprite* explosionRangeSprite_ = nullptr; // 爆炸范围精灵

	// 旋转相关变量
	float rotation_ = 0.0f;           // 当前旋转角度（弧度）
	float rotationSpeed_ = 1.0f;      // 旋转速度（弧度/秒）

	KamataEngine::Vector3 initialPosition_ = { -30, 0, 0 }; // 初始位置
	KamataEngine::Vector3 initialScale_ = { 2, 2, 2 };      // 初始缩放
	float initialRotation_ = 0.0f;                        // 初始旋转

	// 爆炸特效相关变量
	std::vector<uint32_t> explosionTextureHandles_; // 爆炸特效纹理句柄
	KamataEngine::Sprite* explosionSprite_ = nullptr; // 爆炸特效精灵
	bool isExplosionAnimPlaying_ = false;           // 是否正在播放爆炸动画
	int currentExplosionFrame_ = 0;                 // 当前爆炸帧
	float explosionAnimTimer_ = 0.0f;               // 爆炸动画计时器
	const float explosionFrameDuration_ = 0.1f;     // 每帧持续时间（秒）
	const int explosionTotalFrames_ = 4;            // 爆炸总帧数


	// 击退锁定相关变量
	bool isKnockbackLocked_ = false;                // 是否处于击退锁定状态
	float knockbackLockTimer_ = 0.0f;               // 击退锁定计时器
	const float knockbackLockDuration_ = 1.0f;      // 击退锁定持续时间（秒）

	// 新增拖尾特效相关变量
	struct TrailPoint {
		KamataEngine::Vector3 position;
		float lifetime;
		float maxLifetime;
		float alpha;
		KamataEngine::Sprite* sprite; // 每个拖尾点有自己的精灵
	};
	std::vector<TrailPoint> trailPoints_;           // 拖尾点队列
	const int maxTrailPoints_ = 10;                 // 最大拖尾点数
	const float trailSpawnInterval_ = 0.05f;        // 拖尾生成间隔（秒）
	float trailSpawnTimer_ = 0.0f;                  // 拖尾生成计时器
	uint32_t trailTextureHandle_ = 0;               // 拖尾纹理句柄
	const float trailSize_ = 60.0f;                 // 拖尾点大小

	// 私有方法
	KamataEngine::Vector3 WorldToScreen(const KamataEngine::Vector3& worldPos);
	void UpdateKnockbackLock();                     // 更新击退锁定状态
	void UpdateTrail();                            // 更新拖尾特效
	void DrawTrail();                              // 绘制拖尾特效
	void AddTrailPoint();                          // 添加拖尾点
	void CleanupTrail();                           // 清理拖尾资源
};