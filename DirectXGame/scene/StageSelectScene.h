// StageSelectScene.h
#pragma once
#include "KamataEngine.h"
#include "../play/GameUI.h"
#include "IScene.h"
#include "SceneState.h"

class StageSelectScene : public IScene {
public:
    StageSelectScene();
    ~StageSelectScene();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    bool IsSceneEnd() const override { return isSceneEnd_; }
    SceneState GetNextSceneState() const override { return nextSceneState_; }

    int GetSelectedLevel() const { return selectedLevel_; }

private:
    KamataEngine::DirectXCommon* dxCommon_ = nullptr;
    KamataEngine::Input* input_ = nullptr;

    uint32_t backgroundTextureHandle_ = 0;
    KamataEngine::Sprite* backgroundSprite_ = nullptr;

    uint32_t stage1ButtonTextureHandle_ = 0;
    uint32_t stage2ButtonTextureHandle_ = 0;
    KamataEngine::Sprite* stage1ButtonSprite_ = nullptr;
    KamataEngine::Sprite* stage2ButtonSprite_ = nullptr;

    KamataEngine::Vector2 normalButtonSize_ = { 200.0f, 100.0f };
    KamataEngine::Vector2 hoverButtonSize_ = { 220.0f, 110.0f };
    KamataEngine::Vector2 stage1ButtonPosition_ = { 340.0f, 300.0f };
    KamataEngine::Vector2 stage2ButtonPosition_ = { 740.0f, 300.0f };
    bool isMouseOverStage1_ = false;
    bool isMouseOverStage2_ = false;

    GameUI* gameUI_ = nullptr;

    bool isSceneEnd_ = false;
    int selectedLevel_ = 0;          // 0=返回标题, 1=关卡1, 5=关卡2...
    SceneState nextSceneState_ = TITLE; // 默认返回标题

    bool IsMouseOverButton(const KamataEngine::Vector2& mousePos, KamataEngine::Sprite* buttonSprite);
    void UpdateButtonStates();
};