#pragma once

#include "KamataEngine.h"

#include <memory>

using namespace KamataEngine;

class GameUI {
public:
    GameUI();
    ~GameUI();

    void Initialize();
    void Update();
    void Draw();

    bool IsRestartClicked() const { return isRestartClicked_; }
    bool IsReturnToTitleClicked() const { return isReturnToTitleClicked_; }

    void SetShowRestartButton(bool show) { showRestartButton_ = show; }
    void SetShowTitleButton(bool show) { showTitleButton_ = show; }

    void ResetClicks();

private:
    DirectXCommon* dxCommon_ = nullptr;
    Input* input_ = nullptr;

    bool isRestartClicked_ = false;
    bool isReturnToTitleClicked_ = false;

    uint32_t restartTextureHandle_ = 0;
    std::unique_ptr<Sprite> restartSprite_;

    uint32_t titleTextureHandle_ = 0;
    std::unique_ptr<Sprite> titleSprite_;

    Vector2 restartPosition_ = {};
    Vector2 titlePosition_ = {};
    Vector2 normalButtonSize_ = {};
    Vector2 hoverButtonSize_ = {};

    bool showRestartButton_ = true;
    bool showTitleButton_ = true;

    bool isMouseOverRestart_ = false;
    bool isMouseOverTitle_ = false;

private:
    void LoadTextures();
    bool IsMouseOverRestart(const Vector2& mousePos);
    bool IsMouseOverTitle(const Vector2& mousePos);
    void UpdateButtonStates();
};
