// LoadingScene.h
#pragma once
#include "KamataEngine.h"
#include "IScene.h"
#include "SceneState.h"

class LoadingScene : public IScene {
public:
    void Initialize() override;
    void Update() override;      
    void Draw() override;
    bool IsSceneEnd() const override { return isLoadingComplete_; }
    SceneState GetNextSceneState() const override { return GAME; }

    void StartLoading() { isLoadingComplete_ = false; frameCount_ = 0; }

private:
    uint32_t loadingTextureHandle_ = 0;
    KamataEngine::Sprite* loadingSprtie_ = nullptr;
    uint32_t loadingBGTextureHandle_ = 0;
    KamataEngine::Sprite* loadingBGSprtie_ = nullptr;

    KamataEngine::DirectXCommon* dxCommon_ = nullptr;

    bool isLoadingComplete_ = false;
    int frameCount_ = 0;
    const int loadingDuration_ = 120;
};