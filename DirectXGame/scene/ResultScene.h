#pragma once

#include "KamataEngine.h"
#include "IScene.h"
#include "SceneState.h"

#include <memory>

using namespace KamataEngine;

/// <summary>
/// ゲーム終了後のリザルト画面を管理するクラス。
/// </summary>
class ResultScene : public IScene {
public:
    /// <summary>
    /// ResultScene に関する処理を行う。
    /// </summary>
    ResultScene();
    /// <summary>
    /// ~ResultScene に関する処理を行う。
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
    DirectXCommon* dxCommon_ = nullptr;
    Input* input_ = nullptr;

    bool isSceneEnd_ = false;
    int score_ = 0;

    uint32_t resultBackgroundTextureHandle_ = 0;
    std::unique_ptr<Sprite> resultBackgroundSprite_;

    uint32_t resultTextureHandle_ = 0;
    std::unique_ptr<Sprite> resultSprite_;

    uint32_t retryTextureHandle_ = 0;
    std::unique_ptr<Sprite> retrySprite_;

    uint32_t titleTextureHandle_ = 0;
    std::unique_ptr<Sprite> titleSprite_;

    int frameCount_ = 0;

    Vector2 normalButtonSize_ = {};
    Vector2 hoverButtonSize_ = {};
    Vector2 buttonPosition_ = {};
    bool isMouseOverTitle_ = false;

private:
    /// <summary>
    /// LoadTextures に関する処理を行う。
    /// </summary>
    void LoadTextures();
    /// <summary>
    /// IsMouseOverTitle に関する処理を行う。
    /// </summary>
    bool IsMouseOverTitle(const Vector2& mousePos);
    /// <summary>
    /// UpdateButtonState に関する処理を行う。
    /// </summary>
    void UpdateButtonState();
};
