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

// 関数コメント: StartLoading の処理を実行する。
void LoadingScene::StartLoading() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、StartLoading の役割を実現する。
    isLoadingComplete_ = false;
    frameCount_ = 0;
}

// 関数コメント: Initialize の処理を実行する。
void LoadingScene::Initialize() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Initialize の役割を実現する。
    dxCommon_ = DirectXCommon::GetInstance();
    loadingDuration_ = kLoadingDurationFrame;

    loadingTextureHandle_ = TextureManager::Load("loading/loading.png");
    loadingSprite_.reset(Sprite::Create(loadingTextureHandle_, {kLoadingSpriteX, kLoadingSpriteY}));

    loadingBGTextureHandle_ = TextureManager::Load("loading/loadingBG.png");
    loadingBGSprite_.reset(Sprite::Create(loadingBGTextureHandle_, {kLoadingSpriteX, kLoadingSpriteY}));
}

// 関数コメント: Update の処理を実行する。
void LoadingScene::Update() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Update の役割を実現する。
    frameCount_++;

    if (frameCount_ >= loadingDuration_) {
        isLoadingComplete_ = true;
    }
}

// 関数コメント: Draw の処理を実行する。
void LoadingScene::Draw() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Draw の役割を実現する。
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
