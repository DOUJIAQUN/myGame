#include "LoadingScene.h"

using namespace KamataEngine;

namespace {
    constexpr float kLoadingSpriteX = 0.0f;
    constexpr float kLoadingSpriteY = 0.0f;
    constexpr int kLoadingDurationFrame = 120;
    constexpr int kBlinkCycleFrame = 60;
    constexpr int kBlinkVisibleStartFrame = 30;
}

LoadingScene::~LoadingScene() = default;

void LoadingScene::StartLoading() {
    isLoadingComplete_ = false;
    frameCount_ = 0;
}

void LoadingScene::Initialize() {
    dxCommon_ = DirectXCommon::GetInstance();
    loadingDuration_ = kLoadingDurationFrame;

    loadingTextureHandle_ = TextureManager::Load("loading/loading.png");
    loadingSprite_.reset(Sprite::Create(loadingTextureHandle_, {kLoadingSpriteX, kLoadingSpriteY}));

    loadingBGTextureHandle_ = TextureManager::Load("loading/loadingBG.png");
    loadingBGSprite_.reset(Sprite::Create(loadingBGTextureHandle_, {kLoadingSpriteX, kLoadingSpriteY}));
}

void LoadingScene::Update() {
    frameCount_++;

    if (frameCount_ >= loadingDuration_) {
        isLoadingComplete_ = true;
    }
}

void LoadingScene::Draw() {
    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
    Sprite::PreDraw(commandList);

    if (loadingBGSprite_) {
        loadingBGSprite_->Draw();
    }
    if (loadingSprite_ && (frameCount_ % kBlinkCycleFrame) >= kBlinkVisibleStartFrame) {
        loadingSprite_->Draw();
    }

    Sprite::PostDraw();
    dxCommon_->ClearDepthBuffer();
}
