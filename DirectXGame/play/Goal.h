#pragma once
#include "KamataEngine.h"
#include "../math/myMath.h"
#include <functional>

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
	void ResetCount() { enterCount_ = 0; requiredCount_ = 1; }
	void SetRequiredCount(int count) { requiredCount_ = count; }
	void IncrementCount() { enterCount_++; }
	bool IsCompleted() const { return enterCount_ >= requiredCount_; }
	int GetCurrentCount() const { return enterCount_; }
	int GetRequiredCount() const { return requiredCount_; }
	

private:
	KamataEngine::Model* model_;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::ObjectColor objectColor;
	KamataEngine::Camera* camera_;
	KamataEngine::Input* input_ = nullptr;
	const float kSpeed = 0.2f;


	int enterCount_ = 0;        // 当前进入次数
	int requiredCount_ = 1;     // 需要进入的次数
	
};