#pragma once
#include "KamataEngine.h"
#include <functional>
#include "myMath.h"

class Ball

{
public:
	~Ball();
	void Initialize(KamataEngine::Camera* camera);
	void Update();
	void Draw();

	 // 添加 SetPosition 方法
	void SetPosition(const KamataEngine::Vector3& position);

	KamataEngine::Vector3 GetPosition() const { return worldTransform_.translation_; }

	  // 爆炸相关方法
	void Explode();                                               // 触发爆炸
	bool IsExploded() const { return isExploded_; }               // 是否已爆炸
	bool IsActive() const { return isActive_; }                   // 是否活跃（未消失）
	void ApplyExplosionForce(const KamataEngine::Vector3& force); // 应用爆炸力

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
	
};