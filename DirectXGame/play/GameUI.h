#pragma once

#include "KamataEngine.h"

#include <memory>

namespace MyEngine {

/// <summary>
/// ゲーム中のリスタートボタンやタイトルへ戻るボタンなどの UI を管理するクラス。
/// </summary>
class GameUI {
public:
    /// <summary>
/// リスタートボタンとタイトルボタンの Sprite、入力判定、クリック状態を管理する。
/// </summary>
    GameUI();
    /// <summary>
/// GameUI が所有する Sprite を unique_ptr により自動解放する。
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
/// 前フレームの UI クリック状態を消去し、次フレームの入力判定に備える。
/// </summary>
    void ResetClicks();

private:
    KamataEngine::DirectXCommon* dxCommon_ = nullptr;
    KamataEngine::Input* input_ = nullptr;

    bool isRestartClicked_ = false;
    bool isReturnToTitleClicked_ = false;

    uint32_t restartTextureHandle_ = 0;
    std::unique_ptr<KamataEngine::Sprite> restartSprite_;

    uint32_t titleTextureHandle_ = 0;
    std::unique_ptr<KamataEngine::Sprite> titleSprite_;

    KamataEngine::Vector2 restartPosition_ = {};
    KamataEngine::Vector2 titlePosition_ = {};
    KamataEngine::Vector2 normalButtonSize_ = {};
    KamataEngine::Vector2 hoverButtonSize_ = {};

    bool showRestartButton_ = true;
    bool showTitleButton_ = true;

    bool isMouseOverRestart_ = false;
    bool isMouseOverTitle_ = false;

private:
    /// <summary>
/// UI ボタン用テクスチャを読み込み、対応する Sprite を生成する。
/// </summary>
    void LoadTextures();
    /// <summary>
/// マウス座標がリスタートボタンの矩形内にあるか判定する。
/// </summary>
    bool IsMouseOverRestart(const KamataEngine::Vector2& mousePos);
    /// <summary>
/// マウス座標がタイトルボタンの矩形内にあるか判定する。
/// </summary>
    bool IsMouseOverTitle(const KamataEngine::Vector2& mousePos);
    /// <summary>
/// 各ボタンのホバー状態を更新し、クリックされたボタンのフラグを立てる。
/// </summary>
    void UpdateButtonStates();
};

} // namespace MyEngine
