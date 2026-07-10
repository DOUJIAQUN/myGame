#include "Stage.h"

using namespace KamataEngine;

namespace {
    constexpr float kInitialScrollX = 0.0f;
    constexpr float kScrollSpeed = 2.0f;
    constexpr int kScreenWidth = 1280;
    constexpr float kBackgroundY = 0.0f;
}

Stage::~Stage() = default;

// 関数コメント: Initialize の処理を実行する。
void Stage::Initialize() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Initialize の役割を実現する。
    textureHandle_ = TextureManager::Load("stage.png");
    scrollX_ = kInitialScrollX;
    scrollSpeed_ = kScrollSpeed;
    screenWidth_ = kScreenWidth;

    bgSprite1_.reset(Sprite::Create(textureHandle_, {0.0f, kBackgroundY}));
    bgSprite2_.reset(Sprite::Create(textureHandle_, {static_cast<float>(screenWidth_), kBackgroundY}));
}

// 関数コメント: Update の処理を実行する。
void Stage::Update() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Update の役割を実現する。
    // 背景スクロールを使う場合は、下記の処理を有効化する。
    // scrollX_ -= scrollSpeed_;
    // if (scrollX_ <= -screenWidth_) {
    //     scrollX_ += screenWidth_;
    // }
    // bgSprite1_->SetPosition({scrollX_, 0.0f});
    // bgSprite2_->SetPosition({scrollX_ + screenWidth_, 0.0f});
}

// 関数コメント: Draw の処理を実行する。
void Stage::Draw() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Draw の役割を実現する。
    if (bgSprite1_) {
        bgSprite1_->Draw();
    }
    if (bgSprite2_) {
        bgSprite2_->Draw();
    }
}
