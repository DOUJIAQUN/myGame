#include "GameUI.h"

#include "../scene/ButtonUtility.h"

#include <cassert>

using namespace KamataEngine;

namespace {
    const Vector2 kRestartButtonPosition = {1100.0f, 20.0f};
    const Vector2 kTitleButtonPosition = {1180.0f, 20.0f};
    const Vector2 kNormalButtonSize = {64.0f, 64.0f};
    const Vector2 kHoverButtonSize = {70.0f, 70.0f};
    constexpr int kLeftMouseButton = 0;
}

GameUI::GameUI() = default;
GameUI::~GameUI() = default;

void GameUI::Initialize() {
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

void GameUI::LoadTextures() {
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

void GameUI::Update() {
    UpdateButtonStates();

    isRestartClicked_ = false;
    isReturnToTitleClicked_ = false;

    Vector2 mousePos = input_->GetMousePosition();

    if (input_->IsTriggerMouse(kLeftMouseButton)) {
        if (IsMouseOverRestart(mousePos)) {
            isRestartClicked_ = true;
        }
        else if (IsMouseOverTitle(mousePos)) {
            isReturnToTitleClicked_ = true;
        }
    }
}

void GameUI::UpdateButtonStates() {
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

void GameUI::Draw() {
    if (restartSprite_ && showRestartButton_) {
        restartSprite_->Draw();
    }
    if (titleSprite_ && showTitleButton_) {
        titleSprite_->Draw();
    }
}

bool GameUI::IsMouseOverRestart(const Vector2& mousePos) {
    if (!restartSprite_) {
        return false;
    }

    Vector2 position = restartSprite_->GetPosition();
    Vector2 size = restartSprite_->GetSize();

    return (mousePos.x >= position.x && mousePos.x <= position.x + size.x &&
        mousePos.y >= position.y && mousePos.y <= position.y + size.y);
}

bool GameUI::IsMouseOverTitle(const Vector2& mousePos) {
    if (!titleSprite_) {
        return false;
    }

    Vector2 position = titleSprite_->GetPosition();
    Vector2 size = titleSprite_->GetSize();

    return (mousePos.x >= position.x && mousePos.x <= position.x + size.x &&
        mousePos.y >= position.y && mousePos.y <= position.y + size.y);
}

void GameUI::ResetClicks() {
    isRestartClicked_ = false;
    isReturnToTitleClicked_ = false;
}
