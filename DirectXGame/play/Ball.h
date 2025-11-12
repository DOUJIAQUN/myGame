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

	// 修改方法：接收屏幕坐标来更新爆炸范围位置
	void UpdateExplosionRangePosition(const KamataEngine::Vector3& screenPos);


	  // 爆炸相关方法
	void Explode();                                               // 触发爆炸
	bool IsExploded() const { return isExploded_; }               // 是否已爆炸
	bool IsActive() const { return isActive_; }                   // 是否活跃（未消失）
	void ApplyExplosionForce(const KamataEngine::Vector3& force); // 应用爆炸力
	void DrawExplosionRange();

	// 鼠标悬停相关方法
	void SetMouseOver(bool isMouseOver) { isMouseOver_ = isMouseOver; }
	bool IsMouseOver() const { return isMouseOver_; }

	// 新增旋转相关方法
	void SetRotationSpeed(float speed) { rotationSpeed_ = speed; }
	float GetRotation() const { return rotation_; }

	// 修改SetPosition方法，同时更新初始位置
	void SetInitialPosition(const KamataEngine::Vector3& position);
	void SetPosition(const KamataEngine::Vector3& position);

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
};