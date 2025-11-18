#pragma once
#include "KamataEngine.h"
#include "../play/GameUI.h"

class StageSelectScene {
public:
    StageSelectScene();
    ~StageSelectScene();

    void Initialize();
    void Update();
    void Draw();

    bool IsSceneEnd() const { return isSceneEnd_; }
    int GetSelectedLevel() const { return selectedLevel_; }
   

private:
    KamataEngine::DirectXCommon* dxCommon_ = nullptr;
    KamataEngine::Input* input_ = nullptr;

    uint32_t backgroundTextureHandle_ = 0;
    KamataEngine::Sprite* backgroundSprite_ = nullptr;

    // 关卡按钮
    uint32_t stage1ButtonTextureHandle_ = 0;
    uint32_t stage2ButtonTextureHandle_ = 0;
    KamataEngine::Sprite* stage1ButtonSprite_ = nullptr;
    KamataEngine::Sprite* stage2ButtonSprite_ = nullptr;

    KamataEngine::Vector2 normalButtonSize_ = { 200.0f, 100.0f };      // 正常尺寸
    KamataEngine::Vector2 hoverButtonSize_ = { 220.0f, 110.0f };       // 悬停时放大尺寸（约10%）
    KamataEngine::Vector2 stage1ButtonPosition_ = { 340.0f, 300.0f };  // Stage1按钮位置
    KamataEngine::Vector2 stage2ButtonPosition_ = { 740.0f, 300.0f };  // Stage2按钮位置
    bool isMouseOverStage1_ = false;                                   // 鼠标是否在Stage1按钮上
    bool isMouseOverStage2_ = false;                                   // 鼠标是否在Stage2按钮上

    // 使用 GameUI 来处理返回功能
    GameUI* gameUI_ = nullptr;

    bool isSceneEnd_ = false;
    int selectedLevel_ = 0;
   

    bool IsMouseOverButton(const KamataEngine::Vector2& mousePos, KamataEngine::Sprite* buttonSprite);

    void UpdateButtonStates();  //更新按钮状态
};