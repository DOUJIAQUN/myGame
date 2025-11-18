#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class GameUI {
public:
    GameUI();
    ~GameUI();

    void Initialize();
    void Update();
    void Draw();

    // 检测按钮点击
    bool IsRestartClicked() const { return isRestartClicked_; }
    bool IsReturnToTitleClicked() const { return isReturnToTitleClicked_; }
    //控制按钮显示/隐藏
    void SetShowRestartButton(bool show) { showRestartButton_ = show; }
    void SetShowTitleButton(bool show) { showTitleButton_ = show; }

    // 重置点击状态
    void ResetClicks();

private:
    DirectXCommon* dxCommon_ = nullptr;
    Input* input_ = nullptr;

    // 按钮状态
    bool isRestartClicked_ = false;
    bool isReturnToTitleClicked_ = false;

    // 纹理和精灵
    uint32_t restartTextureHandle_ = 0;
    Sprite* restartSprite_ = nullptr;

    uint32_t titleTextureHandle_ = 0;
    Sprite* titleSprite_ = nullptr;

    // 按钮位置和大小
    Vector2 restartPosition_ = { 1100.0f, 50.0f };  // 右上角位置
    Vector2 titlePosition_ = { 1180.0f, 50.0f };    // 在重新开始按钮右侧
    Vector2 normalButtonSize_ = { 64.0f, 64.0f };         // 图标大小
    Vector2 hoverButtonSize_ = { 70.0f, 70.0f };    // 悬停时放大尺寸

    //控制按钮显示
    bool showRestartButton_ = true;
    bool showTitleButton_ = true;

    //鼠标悬停状态
    bool isMouseOverRestart_ = false;
    bool isMouseOverTitle_ = false;
    // 加载纹理
    void LoadTextures();

    // 检测鼠标悬停
    bool IsMouseOverRestart(const Vector2& mousePos);
    bool IsMouseOverTitle(const Vector2& mousePos);

    //更新按钮状态
    void UpdateButtonStates();
};