#include "GameUI.h"

#include "../scene/ButtonUtility.h"

#include <cassert>

namespace {
    const Vector2 kRestartButtonPosition = {1100.0f, 20.0f};
    const Vector2 kTitleButtonPosition = {1180.0f, 20.0f};
    const Vector2 kNormalButtonSize = {64.0f, 64.0f};
    const Vector2 kHoverButtonSize = {70.0f, 70.0f};
    constexpr int kLeftMouseButton = 0;
}

GameUI::GameUI() = default;
GameUI::~GameUI() = default;

// 関数コメント: Initialize の処理を実行する。
void GameUI::Initialize() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Initialize の役割を実現する。
    input_ = Input::GetInstance();
    dxCommon_ = DirectXCommon::GetInstance();

    restartPosition_ = kRestartButtonPosition;
    titlePosition_ = kTitleButtonPosition;
    normalButtonSize_ = kNormalButtonSize;
    hoverButtonSize_ = kHoverButtonSize;

    isRestartClicked_ = false;
    isReturnToTitleClicked_ = false;

    LoadTextures();
}

// 関数コメント: LoadTextures の処理を実行する。
void GameUI::LoadTextures() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、LoadTextures の役割を実現する。
    restartTextureHandle_ = TextureManager::Load("ui/restart.png");
    if (restartTextureHandle_ != 0) {
        restartSprite_.reset(Sprite::Create(restartTextureHandle_, restartPosition_));
        if (restartSprite_) {
            restartSprite_->SetSize(normalButtonSize_);
        }
    }

    titleTextureHandle_ = TextureManager::Load("ui/title.png");
    if (titleTextureHandle_ != 0) {
        titleSprite_.reset(Sprite::Create(titleTextureHandle_, titlePosition_));
        if (titleSprite_) {
            titleSprite_->SetSize(normalButtonSize_);
        }
    }
}

// 関数コメント: Update の処理を実行する。
void GameUI::Update() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Update の役割を実現する。
    UpdateButtonStates();

    isRestartClicked_ = false;
    isReturnToTitleClicked_ = false;

    Vector2 mousePos = input_->GetMousePosition();

    if (input_->IsTriggerMouse(kLeftMouseButton)) {
        if (IsMouseOverRestart(mousePos)) {
            isRestartClicked_ = true;
        }
        // 関数コメント: if の処理を実行する。
        else if (IsMouseOverTitle(mousePos)) {
            // 処理コメント: 必要な状態確認やデータ更新を行い、if の役割を実現する。
            isReturnToTitleClicked_ = true;
        }
    }
}

// 関数コメント: UpdateButtonStates の処理を実行する。
void GameUI::UpdateButtonStates() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、UpdateButtonStates の役割を実現する。
    Vector2 mousePos = input_->GetMousePosition();

    bool wasMouseOverRestart = isMouseOverRestart_;
    bool wasMouseOverTitle = isMouseOverTitle_;

    isMouseOverRestart_ = IsMouseOverRestart(mousePos);
    isMouseOverTitle_ = IsMouseOverTitle(mousePos);

    ButtonUtility::UpdateHoverSprite(
        restartSprite_.get(),
        restartPosition_,
        normalButtonSize_,
        hoverButtonSize_,
        isMouseOverRestart_,
        wasMouseOverRestart
    );

    ButtonUtility::UpdateHoverSprite(
        titleSprite_.get(),
        titlePosition_,
        normalButtonSize_,
        hoverButtonSize_,
        isMouseOverTitle_,
        wasMouseOverTitle
    );
}

// 関数コメント: Draw の処理を実行する。
void GameUI::Draw() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Draw の役割を実現する。
    if (restartSprite_ && showRestartButton_) {
        restartSprite_->Draw();
    }
    if (titleSprite_ && showTitleButton_) {
        titleSprite_->Draw();
    }
}

// 関数コメント: IsMouseOverRestart の処理を実行する。
bool GameUI::IsMouseOverRestart(const Vector2& mousePos) {
    // 処理コメント: 必要な状態確認やデータ更新を行い、IsMouseOverRestart の役割を実現する。
    if (!restartSprite_) {
        return false;
    }

    Vector2 position = restartSprite_->GetPosition();
    Vector2 size = restartSprite_->GetSize();

    return (mousePos.x >= position.x && mousePos.x <= position.x + size.x &&
        mousePos.y >= position.y && mousePos.y <= position.y + size.y);
}

// 関数コメント: IsMouseOverTitle の処理を実行する。
bool GameUI::IsMouseOverTitle(const Vector2& mousePos) {
    // 処理コメント: 必要な状態確認やデータ更新を行い、IsMouseOverTitle の役割を実現する。
    if (!titleSprite_) {
        return false;
    }

    Vector2 position = titleSprite_->GetPosition();
    Vector2 size = titleSprite_->GetSize();

    return (mousePos.x >= position.x && mousePos.x <= position.x + size.x &&
        mousePos.y >= position.y && mousePos.y <= position.y + size.y);
}

// 関数コメント: ResetClicks の処理を実行する。
void GameUI::ResetClicks() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、ResetClicks の役割を実現する。
    isRestartClicked_ = false;
    isReturnToTitleClicked_ = false;
}
