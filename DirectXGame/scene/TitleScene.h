#pragma once
#include "KamataEngine.h"

class TitleScene {
public:
    ~TitleScene();
    void Initialize();
    void Update();
    void Draw();
    bool IsSceneEnd();  
    int GetSelectedLevel() const { return selectedLevel_; }  // 新增：获取选择的关卡

private:
    uint32_t titleTextureHandle_ = 0;
    KamataEngine::Sprite* titleSprite_ = nullptr;
	uint32_t backgroundTextureHandle_ = 0;
	KamataEngine::Sprite* backgroundSprite_ = nullptr;
    uint32_t startButtonTextureHandle_ = 0;
    KamataEngine::Sprite* startButtonSprite_ = nullptr;
    uint32_t stageSelectButtonTextureHandle_ = 0;
    KamataEngine::Sprite* stageSelectButtonSprite_ = nullptr;


    //按钮状态相关变量
    KamataEngine::Vector2 normalButtonSize_ = { 200.0f, 90.0f };      // 正常尺寸
    KamataEngine::Vector2 hoverButtonSize_ = { 220.0f, 99.0f };       // 悬停时放大尺寸（约10%）
    KamataEngine::Vector2 startButtonPosition_ = { 540.0f, 400.0f };   // Start按钮位置
    KamataEngine::Vector2 stageSelectButtonPosition_ = { 540.0f, 520.0f }; // StageSelect按钮位置
    bool isMouseOverStart_ = false;                                    // 鼠标是否在Start按钮上
    bool isMouseOverStageSelect_ = false;                              // 鼠标是否在StageSelect按钮上

    KamataEngine::Input* input_ = nullptr;
    KamataEngine::DirectXCommon* dxCommon_ = nullptr;
    bool isSceneEnd_ = false;
    int frameCount_ = 0;

    int selectedLevel_ = 1; // 0=进入关卡选择, 1=直接开始第一关

    bool IsMouseOverButton(const KamataEngine::Vector2& mousePos, KamataEngine::Sprite* buttonSprite);

    void UpdateButtonStates();  // 新增：更新按钮状态
};
