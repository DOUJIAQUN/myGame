#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class ResultScene {
public:
    ResultScene();
    ~ResultScene();

    void Initialize();
    void Update();
    void Draw();

    bool IsSceneEnd() const { return isSceneEnd_; }
    void SetScore(int score) { score_ = score; }

private:
    DirectXCommon* dxCommon_ = nullptr;
    Input* input_ = nullptr;

    bool isSceneEnd_ = false;
    int score_ = 0;

    uint32_t resultTextureHandle_ = 0;
    Sprite* resultSprite_ = nullptr;

    uint32_t retryTextureHandle_ = 0;
    Sprite* retrySprite_ = nullptr;

    uint32_t titleTextureHandle_ = 0;
    Sprite* titleSprite_ = nullptr;

    void LoadTextures();

    bool IsMouseOverTitle(const Vector2& mousePos);
};