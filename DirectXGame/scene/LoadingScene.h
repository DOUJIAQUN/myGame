// LoadingScene.h
#pragma once

#include "KamataEngine.h"
#include "IScene.h"
#include "SceneState.h"

#include <memory>

/// <summary>
/// シーン切り替え時のロード画面を管理するクラス。
/// </summary>
class LoadingScene : public IScene {
public:
    /// <summary>
    /// ~LoadingScene に関する処理を行う。
    /// </summary>
    ~LoadingScene() override;

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
    bool IsSceneEnd() const override { return isLoadingComplete_; }
    SceneState GetNextSceneState() const override { return GAME; }

    /// <summary>
    /// ロード演出を開始する。
    /// </summary>
    void StartLoading();

private:
    uint32_t loadingTextureHandle_ = 0;
    std::unique_ptr<KamataEngine::Sprite> loadingSprite_;

    uint32_t loadingBGTextureHandle_ = 0;
    std::unique_ptr<KamataEngine::Sprite> loadingBGSprite_;

    KamataEngine::DirectXCommon* dxCommon_ = nullptr;

    bool isLoadingComplete_ = false;
    int frameCount_ = 0;
    int loadingDuration_ = 0;
};
