#pragma once
#include "KamataEngine.h"
#include <functional>
#include <memory>
#include "../math/myMath.h"
#include "Goal.h"

namespace MyEngine {

/// <summary>
/// プレイヤーがクリックして爆発させる球体を管理するクラス。位置、速度、爆発、ノックバック、描画処理を担当する。
/// </summary>
class Ball
{
public:
	static constexpr float kDefaultKnockbackDurationParam = 0.5f;
	static constexpr float kDefaultForceMultiplierParam = 1.0f;
	static constexpr float kDefaultTrailSizeParam = 60.0f;

	/// <summary>
	/// ~Ball に関する処理を行う。
	/// </summary>
	~Ball();
	/// <summary>
	/// オブジェクトやシーンの初期化処理を行う。
	/// </summary>
	void Initialize(KamataEngine::Camera* camera);
	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理を行う。
	/// </summary>
	void Draw();

	/// <summary>
	/// OnEnterGoal に関する処理を行う。
	/// </summary>
	void OnEnterGoal(Goal* goal);

	KamataEngine::Vector3 GetPosition() const { return worldTransform_.translation_; }

	// 接收屏幕坐标来更新爆炸范围位置
	/// <summary>
	/// UpdateExplosionRangePosition に関する処理を行う。
	/// </summary>
	void UpdateExplosionRangePosition(const KamataEngine::Vector3& screenPos);

	// 爆炸相关方法
	/// <summary>
	/// Explode に関する処理を行う。
	/// </summary>
	void Explode();                                               // 触发爆炸
	bool IsExploded() const { return isExploded_; }               // 是否已爆炸
	bool IsActive() const { return isActive_; }                   // 是否活跃（未消失）
	void SetActive(bool active) {
		isActive_ = active;
		if (!active) {
			// 球体不活跃时，立即清空拖尾
			/// <summary>
			/// CleanupTrail に関する処理を行う。
			/// </summary>
			CleanupTrail();
		}
	}           // 设置活跃状态
	/// <summary>
	/// ApplyExplosionForce に関する処理を行う。
	/// </summary>
	void ApplyExplosionForce(const KamataEngine::Vector3& force); // 应用爆炸力
	/// <summary>
	/// DrawExplosionRange に関する処理を行う。
	/// </summary>
	void DrawExplosionRange();

	// 鼠标悬停相关方法
	void SetMouseOver(bool isMouseOver) { isMouseOver_ = isMouseOver; }
	bool IsMouseOver() const { return isMouseOver_; }

	// 新增旋转相关方法
	void SetRotationSpeed(float speed) { rotationSpeed_ = speed; }
	float GetRotation() const { return rotation_; }

	// SetPosition方法，同时更新初始位置
	/// <summary>
	/// SetInitialPosition に関する処理を行う。
	/// </summary>
	void SetInitialPosition(const KamataEngine::Vector3& position);
	/// <summary>
	/// SetPosition に関する処理を行う。
	/// </summary>
	void SetPosition(const KamataEngine::Vector3& position);

	// 检查是否可被点击
	bool IsClickable() const { return isActive_ && !isExploded_ && !isKnockbackLocked_; }

	// 设置击退锁定状态
	void SetKnockbackLocked(bool locked) { isKnockbackLocked_ = locked; }
	bool IsKnockbackLocked() const { return isKnockbackLocked_; }

	/// <summary>
	/// Reset に関する処理を行う。
	/// </summary>
	void Reset();

	// 物理相关方法
	KamataEngine::Vector3 GetVelocity() const { return velocity_; }
	void ApplyForce(const KamataEngine::Vector3& force) {
		velocity_ = myMath::Add(velocity_, force);
	}

	// 统一的击飞方法
	void StartKnockback(const KamataEngine::Vector3& force,
		float duration = kDefaultKnockbackDurationParam,
		float forceMultiplier = kDefaultForceMultiplierParam,
		bool isExplosionKnockback = false);

	// 查询击飞状态
	bool IsKnockedBack() const { return isKnockedBack_; }
	KamataEngine::Vector3 GetKnockbackForce() const { return knockbackForce_; }

private:
	std::unique_ptr<KamataEngine::Model> model_;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::ObjectColor objectColor;
	KamataEngine::Camera* camera_;
	KamataEngine::Input* input_ = nullptr;
	static constexpr float kSpeed = 0.2f;

	// 爆炸相关变量
	bool isExploded_ = false;                    // 是否已爆炸
	bool isActive_ = true;                       // 是否活跃（未消失）
	KamataEngine::Vector3 velocity_ = { 0, 0, 0 }; // 速度向量

	// 鼠标悬停相关变量
	bool isMouseOver_ = false;                   // 鼠标是否悬停
	uint32_t explosionRangeTextureHandle_ = 0;   // 爆炸范围纹理句柄
	std::unique_ptr<KamataEngine::Sprite> explosionRangeSprite_; // 爆炸范围精灵

	// 旋转相关变量
	float rotation_ = 0.0f;           // 当前旋转角度（弧度）
	float rotationSpeed_ = 1.0f;      // 旋转速度（弧度/秒）

	KamataEngine::Vector3 initialPosition_ = {}; // 初始位置
	KamataEngine::Vector3 initialScale_ = {};      // 初始缩放
	float initialRotation_ = 0.0f;                        // 初始旋转

	// 爆炸特效相关变量
	std::vector<uint32_t> explosionTextureHandles_; // 爆炸特效纹理句柄
	std::unique_ptr<KamataEngine::Sprite> explosionSprite_; // 爆炸特效精灵
	bool isExplosionAnimPlaying_ = false;           // 是否正在播放爆炸动画
	int currentExplosionFrame_ = 0;                 // 当前爆炸帧
	float explosionAnimTimer_ = 0.0f;               // 爆炸动画计时器
	static constexpr float kExplosionFrameDuration = 0.1f;     // 每帧持续时间（秒）
	static constexpr int kExplosionTotalFrames = 4;            // 爆炸总帧数

	// 击退锁定相关变量
	bool isKnockbackLocked_ = false;                // 是否处于击退锁定状态
	float knockbackLockTimer_ = 0.0f;               // 击退锁定计时器
	static constexpr float kKnockbackLockDuration = 1.0f;      // 击退锁定持续时间（秒）

	// 新增拖尾特效相关变量
	/// <summary>
	/// Ball の移動軌跡を表示するためのトレイル情報を保持する構造体。
	/// </summary>
	struct TrailPoint {
		KamataEngine::Vector3 position;
		float lifetime;
		float maxLifetime;
		float alpha;
		std::unique_ptr<KamataEngine::Sprite> sprite; // 每个拖尾点有自己的精灵
	};
	std::vector<TrailPoint> trailPoints_;           // 拖尾点队列
	static constexpr int kMaxTrailPoints = 10;                 // 最大拖尾点数
	static constexpr float kTrailSpawnInterval = 0.05f;        // 拖尾生成间隔（秒）
	float trailSpawnTimer_ = 0.0f;                  // 拖尾生成计时器
	uint32_t trailTextureHandle_ = 0;               // 拖尾纹理句柄
	static constexpr float kTrailSize = 60.0f;                 // 拖尾点大小

	// 击飞状态相关
	bool isKnockedBack_ = false;                // 是否正在被击飞
	float knockbackTimer_ = 0.0f;               // 击飞计时器
	float knockbackDuration_ = 0.0f;            // 击飞持续时间
	KamataEngine::Vector3 knockbackForce_ = { 0, 0, 0 }; // 击飞力
	bool isExplosionKnockback_ = false;         // 是否是爆炸击飞

	// 私有方法
	/// <summary>
	/// WorldToScreen に関する処理を行う。
	/// </summary>
	KamataEngine::Vector3 WorldToScreen(const KamataEngine::Vector3& worldPos);
	/// <summary>
	/// UpdateKnockbackLock に関する処理を行う。
	/// </summary>
	void UpdateKnockbackLock();                     // 更新击退锁定状态
	/// <summary>
	/// UpdateTrail に関する処理を行う。
	/// </summary>
	void UpdateTrail();                            // 更新拖尾特效
	/// <summary>
	/// DrawTrail に関する処理を行う。
	/// </summary>
	void DrawTrail();                              // 绘制拖尾特效
	/// <summary>
	/// AddTrailPoint に関する処理を行う。
	/// </summary>
	void AddTrailPoint(float size = kDefaultTrailSizeParam);        // 添加拖尾点
	/// <summary>
	/// CleanupTrail に関する処理を行う。
	/// </summary>
	void CleanupTrail();                           // 清理拖尾资源
	/// <summary>
	/// UpdateKnockback に関する処理を行う。
	/// </summary>
	void UpdateKnockback();                        // 更新击飞状态
	/// <summary>
	/// CalculateSlowDownFactor に関する処理を行う。
	/// </summary>
	float CalculateSlowDownFactor();               // 计算减速因子
	/// <summary>
	/// AddTrailPointWithConfig に関する処理を行う。
	/// </summary>
	void AddTrailPointWithConfig(float size, const KamataEngine::Vector4& color, float lifetime); // 带配置的拖尾点
};

} // namespace MyEngine
