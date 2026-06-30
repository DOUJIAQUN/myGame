#pragma once

#include "KamataEngine.h"
#include "IScene.h"
#include "SceneState.h"

#include <memory>

using namespace KamataEngine;

class ResultScene : public IScene {
public:
    ResultScene();
    ~ResultScene();

    void Initialize() override;
    void Update() override;
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
    void LoadTextures();
    bool IsMouseOverTitle(const Vector2& mousePos);
    void UpdateButtonState();
};
