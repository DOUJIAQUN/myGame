#pragma once

#include "KamataEngine.h"

#include <memory>

/// <summary>
/// ゲーム背景やステージ表示を管理するクラス。
/// </summary>
class Stage {
public:
    /// <summary>
    /// ~Stage に関する処理を行う。
    /// </summary>
    ~Stage();
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

private:
    std::unique_ptr<KamataEngine::Sprite> bgSprite1_;
    std::unique_ptr<KamataEngine::Sprite> bgSprite2_;

    float scrollX_ = 0.0f;
    float scrollSpeed_ = 0.0f;
    int screenWidth_ = 0;
    uint32_t textureHandle_ = 0;
};
