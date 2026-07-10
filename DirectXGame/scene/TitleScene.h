// TitleScene.h
#pragma once

#include "KamataEngine.h"
#include "IScene.h"
#include "SceneState.h"

#include <memory>

/// <summary>
/// タイトル画面の表示、ボタン入力、次シーンへの遷移を管理するクラス。
/// </summary>
class TitleScene : public IScene {
public:
    /// <summary>
    /// ~TitleScene に関する処理を行う。
    /// </summary>
    ~TitleScene();
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
    SceneState GetNextSceneState() const override { return nextSceneState_; }

    int GetSelectedLevel() const { return selectedLevel_; }

private:
    uint32_t titleTextureHandle_ = 0;
    std::unique_ptr<KamataEngine::Sprite> titleSprite_;

    uint32_t backgroundTextureHandle_ = 0;
    std::unique_ptr<KamataEngine::Sprite> backgroundSprite_;

    uint32_t startButtonTextureHandle_ = 0;
    std::unique_ptr<KamataEngine::Sprite> startButtonSprite_;

    uint32_t stageSelectButtonTextureHandle_ = 0;
    std::unique_ptr<KamataEngine::Sprite> stageSelectButtonSprite_;

    KamataEngine::Vector2 normalButtonSize_ = {};
    KamataEngine::Vector2 hoverButtonSize_ = {};
    KamataEngine::Vector2 startButtonPosition_ = {};
    KamataEngine::Vector2 stageSelectButtonPosition_ = {};

    bool isMouseOverStart_ = false;
    bool isMouseOverStageSelect_ = false;

    KamataEngine::Input* input_ = nullptr;
    KamataEngine::DirectXCommon* dxCommon_ = nullptr;
    bool isSceneEnd_ = false;
    int frameCount_ = 0;

    int selectedLevel_ = 1;
    SceneState nextSceneState_ = LOADING;

    /// <summary>
    /// IsMouseOverButton に関する処理を行う。
    /// </summary>
    bool IsMouseOverButton(const KamataEngine::Vector2& mousePos, KamataEngine::Sprite* buttonSprite);
    /// <summary>
    /// UpdateButtonStates に関する処理を行う。
    /// </summary>
    void UpdateButtonStates();
};
