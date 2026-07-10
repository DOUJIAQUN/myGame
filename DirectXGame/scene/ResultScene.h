#pragma once

#include "KamataEngine.h"
#include "IScene.h"
#include "SceneState.h"

#include <memory>

namespace MyEngine {

/// <summary>
/// ゲーム終了後のリザルト画面を管理するクラス。
/// </summary>
class ResultScene : public IScene {
public:
    /// <summary>
/// リザルト画面で表示する背景、結果画像、ボタン状態を初期化する。
/// </summary>
    ResultScene();
    /// <summary>
/// ResultScene が所有する Sprite を unique_ptr により解放する。
/// </summary>
    ~ResultScene();

    /// <summary>
    /// オブジェクトやシーンの初期化処理を行う。
    /// </summary>
    void Initialize() override;
    /// <summary>
    /// 毎フレームの更新処理を行う。
    /// </summary>
    void Update() override;
    /// <summary>
    /// 描画処理を行う。
    /// </summary>
    void Draw() override;

    bool IsSceneEnd() const override { return isSceneEnd_; }
    SceneState GetNextSceneState() const override { return TITLE; }
    void SetScore(int score) { score_ = score; }

private:
    KamataEngine::DirectXCommon* dxCommon_ = nullptr;
    KamataEngine::Input* input_ = nullptr;

    bool isSceneEnd_ = false;
    int score_ = 0;

    uint32_t resultBackgroundTextureHandle_ = 0;
    std::unique_ptr<KamataEngine::Sprite> resultBackgroundSprite_;

    uint32_t resultTextureHandle_ = 0;
    std::unique_ptr<KamataEngine::Sprite> resultSprite_;

    uint32_t retryTextureHandle_ = 0;
    std::unique_ptr<KamataEngine::Sprite> retrySprite_;

    uint32_t titleTextureHandle_ = 0;
    std::unique_ptr<KamataEngine::Sprite> titleSprite_;

    int frameCount_ = 0;

    KamataEngine::Vector2 normalButtonSize_ = {};
    KamataEngine::Vector2 hoverButtonSize_ = {};
    KamataEngine::Vector2 buttonPosition_ = {};
    bool isMouseOverTitle_ = false;

private:
    /// <summary>
/// UI ボタン用テクスチャを読み込み、対応する Sprite を生成する。
/// </summary>
    void LoadTextures();
    /// <summary>
/// マウス座標がタイトルボタンの矩形内にあるか判定する。
/// </summary>
    bool IsMouseOverTitle(const KamataEngine::Vector2& mousePos);
    /// <summary>
/// リザルト画面のボタンホバー状態とクリック判定を更新する。
/// </summary>
    void UpdateButtonState();
};

} // namespace MyEngine
