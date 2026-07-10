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
/// Ball が所有する Model や Sprite を unique_ptr により自動解放する。
/// </summary>
	~Ball();
	/// <summary>
/// Ball が使用するモデル、爆発用 Sprite、初期座標、物理状態を初期化する。
/// </summary>
/// <param name="camera">3D 描画とスクリーン座標変換に使用するカメラ。</param>
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
/// Ball が Goal に入った時、対象 Goal の到達カウントを 1 回増やす。
/// </summary>
/// <param name="goal">接触した Goal。nullptr の場合は何もしない。</param>
	void OnEnterGoal(Goal* goal);

	KamataEngine::Vector3 GetPosition() const { return worldTransform_.translation_; }

	// 接收屏幕坐标来更新爆炸范围位置
	/// <summary>
/// 爆発範囲 Sprite を Ball のスクリーン座標に合わせて移動する。
/// </summary>
/// <param name="screenPos">WorldToScreen で変換した画面上の座標。</param>
	void UpdateExplosionRangePosition(const KamataEngine::Vector3& screenPos);

	// 爆炸相关方法
	/// <summary>
/// クリックされた Ball を爆発済み状態にし、自身の表示やエフェクト開始状態を切り替える。
/// </summary>
	void Explode();                                               // 触发爆炸
	bool IsExploded() const { return isExploded_; }               // 是否已爆炸
	bool IsActive() const { return isActive_; }                   // 是否活跃（未消失）
	void SetActive(bool active) {
		isActive_ = active;
		if (!active) {
			// 球体不活跃时，立即清空拖尾
			/// <summary>
/// Ball が持つトレイル Sprite の配列をクリアし、表示残りを消去する。
/// </summary>
			CleanupTrail();
		}
	}           // 设置活跃状态
	/// <summary>
/// 爆発によって受けた力をノックバックとして Ball に適用する。
/// </summary>
/// <param name="force">爆発中心から Ball へ向かう力ベクトル。</param>
	void ApplyExplosionForce(const KamataEngine::Vector3& force); // 应用爆炸力
	/// <summary>
/// マウスオーバー中の Ball に対して、クリック可能な爆発範囲を 2D 表示する。
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
/// リスタート時に戻る初期座標を設定し、現在座標も同じ値へ更新する。
/// </summary>
/// <param name="position">Ball の初期ワールド座標。</param>
	void SetInitialPosition(const KamataEngine::Vector3& position);
	/// <summary>
/// Ball の現在座標を変更し、WorldTransform に反映する。
/// </summary>
/// <param name="position">更新後のワールド座標。</param>
	void SetPosition(const KamataEngine::Vector3& position);

	// 检查是否可被点击
	bool IsClickable() const { return isActive_ && !isExploded_ && !isKnockbackLocked_; }

	// 设置击退锁定状态
	void SetKnockbackLocked(bool locked) { isKnockbackLocked_ = locked; }
	bool IsKnockbackLocked() const { return isKnockbackLocked_; }

	/// <summary>
/// 指定された力と時間で Ball をノックバック状態にする。
/// </summary>
/// <param name="force">移動方向と強さを表す力ベクトル。</param>
/// <param name="duration">ノックバックを継続する秒数。</param>
/// <param name="forceMultiplier">力に掛ける倍率。</param>
/// <param name="isExplosionKnockback">爆発によるノックバックかどうか。</param>
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
/// Camera の view-projection 行列を使い、3D 座標を画面上の 2D 座標へ変換する。
/// </summary>
	KamataEngine::Vector3 WorldToScreen(const KamataEngine::Vector3& worldPos);
	/// <summary>
/// 衝突直後に再クリック・再衝突が起きないよう、ノックバックのロック時間を更新する。
/// </summary>
	void UpdateKnockbackLock();                     // 更新击退锁定状态
	/// <summary>
/// Ball の移動速度と状態に応じてトレイル点の生成・寿命更新・削除を行う。
/// </summary>
	void UpdateTrail();                            // 更新拖尾特效
	/// <summary>
/// 保持しているトレイル Sprite を古い順に描画し、Ball の移動軌跡を表現する。
/// </summary>
	void DrawTrail();                              // 绘制拖尾特效
	/// <summary>
/// 現在の Ball 位置に標準設定のトレイル点を追加する。
/// </summary>
	void AddTrailPoint(float size = kDefaultTrailSizeParam);        // 添加拖尾点
	/// <summary>
/// Ball が持つトレイル Sprite の配列をクリアし、表示残りを消去する。
/// </summary>
	void CleanupTrail();                           // 清理拖尾资源
	/// <summary>
/// ノックバック中の速度、位置、減速率、終了条件を更新する。
/// </summary>
	void UpdateKnockback();                        // 更新击飞状态
	/// <summary>
/// 爆発・衝突・通常移動の種類に応じた減速率を返す。
/// </summary>
	float CalculateSlowDownFactor();               // 计算减速因子
	/// <summary>
/// サイズ、色、寿命を指定してトレイル点を生成し、画面座標へ配置する。
/// </summary>
	void AddTrailPointWithConfig(float size, const KamataEngine::Vector4& color, float lifetime); // 带配置的拖尾点
};

} // namespace MyEngine
