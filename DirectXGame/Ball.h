#pragma once
#include "KamataEngine.h"
#include <functional>

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

private:
	KamataEngine::Model* model_;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::ObjectColor objectColor;
	KamataEngine::Camera* camera_;
	KamataEngine::Input* input_ = nullptr;
	const float kSpeed = 0.2f;


	
};