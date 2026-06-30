// LoadingScene.h
#pragma once

#include "KamataEngine.h"
#include "IScene.h"
#include "SceneState.h"

#include <memory>

class LoadingScene : public IScene {
public:
    ~LoadingScene() override;

    void Initialize() override;
    void Update() override;
    void Draw() override;
    bool IsSceneEnd() const override { return isLoadingComplete_; }
    SceneState GetNextSceneState() const override { return GAME; }

    void StartLoading();

private:
    uint32_t loadingTextureHandle_ = 0;
    std::unique_ptr<KamataEngine::Sprite> loadingSprite_;

    uint32_t loadingBGTextureHandle_ = 0;
    std::unique_ptr<KamataEngine::Sprite> loadingBGSprite_;

    KamataEngine::DirectXCommon* dxCommon_ = nullptr;

    bool isLoadingComplete_ = false;
    int frameCount_ = 0;
    int loadingDuration_ = 0;
};
