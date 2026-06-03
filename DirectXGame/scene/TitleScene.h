// TitleScene.h
#pragma once
#include "KamataEngine.h"
#include "IScene.h"
#include "SceneState.h"

class TitleScene : public IScene {
public:
    ~TitleScene();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    bool IsSceneEnd() const override { return isSceneEnd_; }
    SceneState GetNextSceneState() const override { return nextSceneState_; }

    int GetSelectedLevel() const { return selectedLevel_; }

private:
    uint32_t titleTextureHandle_ = 0;
    KamataEngine::Sprite* titleSprite_ = nullptr;
    uint32_t backgroundTextureHandle_ = 0;
    KamataEngine::Sprite* backgroundSprite_ = nullptr;
    uint32_t startButtonTextureHandle_ = 0;
    KamataEngine::Sprite* startButtonSprite_ = nullptr;
    uint32_t stageSelectButtonTextureHandle_ = 0;
    KamataEngine::Sprite* stageSelectButtonSprite_ = nullptr;

    KamataEngine::Vector2 normalButtonSize_ = { 200.0f, 90.0f };
    KamataEngine::Vector2 hoverButtonSize_ = { 220.0f, 99.0f };
    KamataEngine::Vector2 startButtonPosition_ = { 540.0f, 400.0f };
    KamataEngine::Vector2 stageSelectButtonPosition_ = { 540.0f, 520.0f };
    bool isMouseOverStart_ = false;
    bool isMouseOverStageSelect_ = false;

    KamataEngine::Input* input_ = nullptr;
    KamataEngine::DirectXCommon* dxCommon_ = nullptr;
    bool isSceneEnd_ = false;
    int frameCount_ = 0;

    int selectedLevel_ = 1;       // 0=进入关卡选择, 1=直接开始第一关
    SceneState nextSceneState_ = LOADING; // 默认下一场景

    bool IsMouseOverButton(const KamataEngine::Vector2& mousePos, KamataEngine::Sprite* buttonSprite);
    void UpdateButtonStates();
};