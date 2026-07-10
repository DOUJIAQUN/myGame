#pragma once
#include "KamataEngine.h"
#include "../math/myMath.h"
#include <functional>
#include <memory>

#include "../play/GoalMovementConfig.h"
#include "../play/IGoalMoveStrategy.h"

namespace MyEngine {

/// <summary>
/// Ball が到達する目標地点を管理するクラス。到達回数、移動設定、達成判定を担当する。
/// </summary>
class Goal
{
public:
	/// <summary>
	/// ~Goal に関する処理を行う。
	/// </summary>
	~Goal();
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

	// 添加 SetPosition 方法
	/// <summary>
	/// SetPosition に関する処理を行う。
	/// </summary>
	void SetPosition(const KamataEngine::Vector3& position);

	KamataEngine::Vector3 GetPosition() const { return worldTransform_.translation_; }

	// 新增方法
	void ResetCount() { enterCount_ = 0; }
	void SetRequiredCount(int count) { requiredCount_ = count; }
	void IncrementCount() { enterCount_++; }
	bool IsCompleted() const { return enterCount_ >= requiredCount_; }
	int GetCurrentCount() const { return enterCount_; }
	int GetRequiredCount() const { return requiredCount_; }

	// 移动相关方法，使用配置
	/// <summary>
	/// SetMovementConfig に関する処理を行う。
	/// </summary>
	void SetMovementConfig(const GoalMovementConfig& config);
	const GoalMovementConfig& GetMovementConfig() const { return movementConfig_; }

private:
	// 根据配置选择 Strategy
	/// <summary>
	/// SetMoveStrategyByConfig に関する処理を行う。
	/// </summary>
	void SetMoveStrategyByConfig();

private:
	std::unique_ptr<KamataEngine::Model> model_;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::ObjectColor objectColor;
	KamataEngine::Camera* camera_;
	KamataEngine::Input* input_ = nullptr;
	static constexpr float kSpeed = 0.2f;

	int enterCount_ = 0;        // 当前进入次数
	int requiredCount_ = 1;     // 需要进入的次数

	// 移动相关变量
	GoalMovementConfig movementConfig_;            // 移动配置
	float moveTimer_ = 0.0f;                       // 移动计时器
	KamataEngine::Vector3 initialPosition_;        // 初始位置

	// Strategy Pattern: 移動アルゴリズムを差し替える
	std::unique_ptr<IGoalMoveStrategy> moveStrategy_;
};

} // namespace MyEngine
