#pragma once

#include "KamataEngine.h"

#include <memory>

using namespace KamataEngine;

/// <summary>
/// ゲーム中のリスタートボタンやタイトルへ戻るボタンなどの UI を管理するクラス。
/// </summary>
class GameUI {
public:
    /// <summary>
    /// GameUI に関する処理を行う。
    /// </summary>
    GameUI();
    /// <summary>
    /// ~GameUI に関する処理を行う。
    /// </summary>
    ~GameUI();

    /// <summary>
    /// オブジェクトやシーンの初期化処理を行う。
    /// </summary>
    void Initialize();
    /// <summary>
    /// 毎フレームの更新処理を行う。
    /// </summary>
    void Update();
    /// <summary>
    /// 描画処理を行う。
    /// </summary>
    void Draw();

    bool IsRestartClicked() const { return isRestartClicked_; }
    bool IsReturnToTitleClicked() const { return isReturnToTitleClicked_; }

    void SetShowRestartButton(bool show) { showRestartButton_ = show; }
    void SetShowTitleButton(bool show) { showTitleButton_ = show; }

    /// <summary>
    /// ResetClicks に関する処理を行う。
    /// </summary>
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
    /// <summary>
    /// LoadTextures に関する処理を行う。
    /// </summary>
    void LoadTextures();
    /// <summary>
    /// IsMouseOverRestart に関する処理を行う。
    /// </summary>
    bool IsMouseOverRestart(const Vector2& mousePos);
    /// <summary>
    /// IsMouseOverTitle に関する処理を行う。
    /// </summary>
    bool IsMouseOverTitle(const Vector2& mousePos);
    /// <summary>
    /// UpdateButtonStates に関する処理を行う。
    /// </summary>
    void UpdateButtonStates();
};
