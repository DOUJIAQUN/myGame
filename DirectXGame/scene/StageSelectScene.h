// StageSelectScene.h
#pragma once

#include "KamataEngine.h"
#include "../play/GameUI.h"
#include "IScene.h"
#include "SceneState.h"

#include <memory>

/// <summary>
/// プレイヤーが開始ステージを選択する画面を管理するクラス。
/// </summary>
class StageSelectScene : public IScene {
public:
    /// <summary>
    /// StageSelectScene に関する処理を行う。
    /// </summary>
    StageSelectScene();
    /// <summary>
    /// ~StageSelectScene に関する処理を行う。
    /// </summary>
    ~StageSelectScene();

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
    KamataEngine::DirectXCommon* dxCommon_ = nullptr;
    KamataEngine::Input* input_ = nullptr;

    uint32_t backgroundTextureHandle_ = 0;
    std::unique_ptr<KamataEngine::Sprite> backgroundSprite_;

    uint32_t stage1ButtonTextureHandle_ = 0;
    uint32_t stage2ButtonTextureHandle_ = 0;
    std::unique_ptr<KamataEngine::Sprite> stage1ButtonSprite_;
    std::unique_ptr<KamataEngine::Sprite> stage2ButtonSprite_;

    KamataEngine::Vector2 normalButtonSize_ = {};
    KamataEngine::Vector2 hoverButtonSize_ = {};
    KamataEngine::Vector2 stage1ButtonPosition_ = {};
    KamataEngine::Vector2 stage2ButtonPosition_ = {};

    bool isMouseOverStage1_ = false;
    bool isMouseOverStage2_ = false;

    std::unique_ptr<GameUI> gameUI_;

    bool isSceneEnd_ = false;
    int selectedLevel_ = 0;
    SceneState nextSceneState_ = TITLE;

    /// <summary>
    /// IsMouseOverButton に関する処理を行う。
    /// </summary>
    bool IsMouseOverButton(const KamataEngine::Vector2& mousePos, KamataEngine::Sprite* buttonSprite);
    /// <summary>
    /// UpdateButtonStates に関する処理を行う。
    /// </summary>
    void UpdateButtonStates();
};
