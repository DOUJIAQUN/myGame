#pragma once

#include "KamataEngine.h"

#include <memory>

class Stage {
public:
    ~Stage();
    void Initialize();
    void Update();
    void Draw();

private:
    std::unique_ptr<KamataEngine::Sprite> bgSprite1_;
    std::unique_ptr<KamataEngine::Sprite> bgSprite2_;

    float scrollX_ = 0.0f;
    float scrollSpeed_ = 0.0f;
    int screenWidth_ = 0;
    uint32_t textureHandle_ = 0;
};
