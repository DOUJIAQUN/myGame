#include "Stage.h"

using namespace KamataEngine;

namespace {
    constexpr float kInitialScrollX = 0.0f;
    constexpr float kScrollSpeed = 2.0f;
    constexpr int kScreenWidth = 1280;
    constexpr float kBackgroundY = 0.0f;
}

Stage::~Stage() = default;

void Stage::Initialize() {
    textureHandle_ = TextureManager::Load("stage.png");
    scrollX_ = kInitialScrollX;
    scrollSpeed_ = kScrollSpeed;
    screenWidth_ = kScreenWidth;

    bgSprite1_.reset(Sprite::Create(textureHandle_, {0.0f, kBackgroundY}));
    bgSprite2_.reset(Sprite::Create(textureHandle_, {static_cast<float>(screenWidth_), kBackgroundY}));
}

void Stage::Update() {
    // 背景スクロールを使う場合は、下記の処理を有効化する。
    // scrollX_ -= scrollSpeed_;
    // if (scrollX_ <= -screenWidth_) {
    //     scrollX_ += screenWidth_;
    // }
    // bgSprite1_->SetPosition({scrollX_, 0.0f});
    // bgSprite2_->SetPosition({scrollX_ + screenWidth_, 0.0f});
}

void Stage::Draw() {
    if (bgSprite1_) {
        bgSprite1_->Draw();
    }
    if (bgSprite2_) {
        bgSprite2_->Draw();
    }
}
