#pragma once
#include "KamataEngine.h"

class Stage {
public:
	~Stage();
	void Initialize();
	void Update();
	void Draw();

private:
	KamataEngine::Sprite* bgSprite1_ = nullptr;
	KamataEngine::Sprite* bgSprite2_ = nullptr;

	float scrollX_ = 0.0f;
	const float scrollSpeed_ = 2.0f;
	const int screenWidth_ = 1280;
	uint32_t textureHandle_ = 0;
};