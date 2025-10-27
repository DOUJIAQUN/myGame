#pragma once
#include "KamataEngine.h"

class LoadingScene {
public:
	void Initialize();
	void Updata();
	void Draw();
	bool isLoadingComplete() { return isLoadingComplete_; }
	void StartLoading() {isLoadingComplete_ = false; frameCount_ = 0;}

private:
	uint32_t loadingTextureHandle_ = 0;
	KamataEngine::Sprite* loadingSprtie_ = nullptr;
	uint32_t loadingBGTextureHandle_ = 0;
	KamataEngine::Sprite* loadingBGSprtie_ = nullptr;

	KamataEngine::DirectXCommon* dxCommon_ = nullptr;

	bool isLoadingComplete_ = false;
	int frameCount_ = 0;
	const int loadingDuration_ = 180;
};