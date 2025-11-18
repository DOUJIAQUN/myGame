#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class ResultScene {
public:
    ResultScene();
    ~ResultScene();

    void Initialize();
    void Update();
    void Draw();

    bool IsSceneEnd() const { return isSceneEnd_; }
    void SetScore(int score) { score_ = score; }

private:
    DirectXCommon* dxCommon_ = nullptr;
    Input* input_ = nullptr;

    bool isSceneEnd_ = false;
    int score_ = 0;

    uint32_t resultBackgroundTextureHandle_ = 0;
    Sprite* resultBackgroundSprite_ = nullptr;

    uint32_t resultTextureHandle_ = 0;
    Sprite* resultSprite_ = nullptr;

    uint32_t retryTextureHandle_ = 0;
    Sprite* retrySprite_ = nullptr;

    uint32_t titleTextureHandle_ = 0;
    Sprite* titleSprite_ = nullptr;

    int frameCount_ = 0;

    // 新增：按钮状态相关变量
    Vector2 normalButtonSize_ = { 200.0f, 90.0f };      // 正常尺寸
    Vector2 hoverButtonSize_ = { 220.0f, 99.0f };       // 悬停时放大尺寸（约10%）
    Vector2 buttonPosition_ = { 540.0f, 520.0f };       // 按钮位置
    bool isMouseOverTitle_ = false;                      // 鼠标是否在按钮上

    void LoadTextures();

    bool IsMouseOverTitle(const Vector2& mousePos);

    void UpdateButtonState();  // 新增：更新按钮状态
};