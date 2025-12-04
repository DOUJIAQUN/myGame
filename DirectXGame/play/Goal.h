#pragma once
#include "KamataEngine.h"
#include "../math/myMath.h"
#include <functional>
#include "../play/GoalMovementConfig.h"
class Goal

{
public:
	~Goal();
	void Initialize(KamataEngine::Camera* camera);
	void Update();
	void Draw();

	// 添加 SetPosition 方法
	void SetPosition(const KamataEngine::Vector3& position);

	KamataEngine::Vector3 GetPosition() const { return worldTransform_.translation_; }


	// 新增方法
	void ResetCount() { enterCount_ = 0;  }
	void SetRequiredCount(int count) { requiredCount_ = count; }
	void IncrementCount() { enterCount_++; }
	bool IsCompleted() const { return enterCount_ >= requiredCount_; }
	int GetCurrentCount() const { return enterCount_; }
	int GetRequiredCount() const { return requiredCount_; }
	
	// 移动相关方法，使用配置
	void SetMovementConfig(const GoalMovementConfig& config);
	const GoalMovementConfig& GetMovementConfig() const { return movementConfig_; }
private:
	KamataEngine::Model* model_;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::ObjectColor objectColor;
	KamataEngine::Camera* camera_;
	KamataEngine::Input* input_ = nullptr;
	const float kSpeed = 0.2f;


	int enterCount_ = 0;        // 当前进入次数
	int requiredCount_ = 1;     // 需要进入的次数
	
	// 移动相关变量
	GoalMovementConfig movementConfig_; // 移动配置
	float moveTimer_ = 0.0f;            // 移动计时器
	KamataEngine::Vector3 initialPosition_; // 初始位置

};