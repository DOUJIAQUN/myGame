#include "LoadingScene.h"

using namespace KamataEngine;

void LoadingScene::Initialize() { 
	dxCommon_ = DirectXCommon::GetInstance();
	loadingTextureHandle_ = TextureManager::Load("loading/loading.png");
	loadingSprtie_ = Sprite::Create(loadingTextureHandle_, {0.0});

	loadingBGTextureHandle_ = TextureManager::Load("loading/loadingBG.png");
	loadingBGSprtie_ = Sprite::Create(loadingBGTextureHandle_, {0.0});
}

void LoadingScene::Updata() { 
	frameCount_++;

	if (frameCount_ >= loadingDuration_) {
		isLoadingComplete_ = true;

	}

}

void LoadingScene::Draw() { 
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	Sprite::PreDraw(commandList);
	
	loadingBGSprtie_->Draw();
	if ((frameCount_ % 60) >= 30) {
		loadingSprtie_->Draw();
	}
	Sprite::PostDraw();

	dxCommon_->ClearDepthBuffer();
}