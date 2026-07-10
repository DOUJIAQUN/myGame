#include "ResultScene.h"
#include "ButtonUtility.h"

#include <cassert>
#include <cmath>

using namespace KamataEngine;

namespace {
    const Vector2 kNormalButtonSize = {200.0f, 90.0f};
    const Vector2 kHoverButtonSize = {220.0f, 99.0f};
    const Vector2 kTitleButtonPosition = {540.0f, 520.0f};
    constexpr int kLeftMouseButton = 0;
    constexpr float kTitleWaveSpeed = 0.05f;
    constexpr float kTitleWaveAmplitude = 10.0f;
    constexpr float kResultTitlePositionX = 20.0f;
    constexpr float kResultTitlePositionY = 20.0f;
}

ResultScene::ResultScene() = default;
ResultScene::~ResultScene() = default;

// 関数コメント: Initialize の処理を実行する。
void ResultScene::Initialize() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Initialize の役割を実現する。
    input_ = Input::GetInstance();
    dxCommon_ = DirectXCommon::GetInstance();

    normalButtonSize_ = kNormalButtonSize;
    hoverButtonSize_ = kHoverButtonSize;
    buttonPosition_ = kTitleButtonPosition;

    isSceneEnd_ = false;
    score_ = 0;
    isMouseOverTitle_ = false;
    frameCount_ = 0;

    LoadTextures();
}

// 関数コメント: LoadTextures の処理を実行する。
void ResultScene::LoadTextures() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、LoadTextures の役割を実現する。
    resultBackgroundTextureHandle_ = TextureManager::Load("result/resultBackground.png");
    resultBackgroundSprite_.reset(Sprite::Create(resultBackgroundTextureHandle_, {0.0f, 0.0f}));

    resultTextureHandle_ = TextureManager::Load("result/resultTitle.png");
    resultSprite_.reset(Sprite::Create(resultTextureHandle_, {kResultTitlePositionX, kResultTitlePositionY}));

    titleTextureHandle_ = TextureManager::Load("result/titleButton.png");
    if (titleTextureHandle_ != 0) {
        titleSprite_.reset(Sprite::Create(titleTextureHandle_, buttonPosition_));
        if (titleSprite_) {
            titleSprite_->SetSize(normalButtonSize_);
        }
    }
}

// 関数コメント: Update の処理を実行する。
void ResultScene::Update() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Update の役割を実現する。
    if (isSceneEnd_) {
        return;
    }

    frameCount_++;

    float offsetY = std::sin(frameCount_ * kTitleWaveSpeed) * kTitleWaveAmplitude;
    if (resultSprite_) {
        resultSprite_->SetPosition({kResultTitlePositionX, kResultTitlePositionY + offsetY});
    }

    UpdateButtonState();

    if (input_->IsTriggerMouse(kLeftMouseButton)) {
        if (isMouseOverTitle_) {
            isSceneEnd_ = true;
        }
    }
}

// 関数コメント: UpdateButtonState の処理を実行する。
void ResultScene::UpdateButtonState() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、UpdateButtonState の役割を実現する。
    Vector2 mousePos = input_->GetMousePosition();

    bool wasMouseOver = isMouseOverTitle_;
    isMouseOverTitle_ = IsMouseOverTitle(mousePos);

    ButtonUtility::UpdateHoverSprite(
        titleSprite_.get(),
        buttonPosition_,
        normalButtonSize_,
        hoverButtonSize_,
        isMouseOverTitle_,
        wasMouseOver
    );
}

// 関数コメント: Draw の処理を実行する。
void ResultScene::Draw() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Draw の役割を実現する。
    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
    Sprite::PreDraw(commandList);

    if (resultBackgroundSprite_) {
        resultBackgroundSprite_->Draw();
    }
    if (resultSprite_) {
        resultSprite_->Draw();
    }
    if (titleSprite_) {
        titleSprite_->Draw();
    }

    Sprite::PostDraw();
    dxCommon_->ClearDepthBuffer();
}

// 関数コメント: IsMouseOverTitle の処理を実行する。
bool ResultScene::IsMouseOverTitle(const Vector2& mousePos) {
    // 処理コメント: 必要な状態確認やデータ更新を行い、IsMouseOverTitle の役割を実現する。
    if (!titleSprite_) {
        return false;
    }

    Vector2 position = titleSprite_->GetPosition();
    Vector2 size = titleSprite_->GetSize();

    return (mousePos.x >= position.x && mousePos.x <= position.x + size.x &&
        mousePos.y >= position.y && mousePos.y <= position.y + size.y);
}
