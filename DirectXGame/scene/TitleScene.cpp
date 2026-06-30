#include "TitleScene.h"
#include "ButtonUtility.h"

#include <cmath>

using namespace KamataEngine;

namespace {
    const Vector2 kNormalButtonSize = {200.0f, 90.0f};
    const Vector2 kHoverButtonSize = {220.0f, 99.0f};
    const Vector2 kStartButtonPosition = {540.0f, 400.0f};
    const Vector2 kStageSelectButtonPosition = {540.0f, 520.0f};
    constexpr int kDefaultSelectedLevel = 1;
    constexpr int kStageSelectSelectedLevel = 0;
    constexpr int kLeftMouseButton = 0;
    constexpr float kTitleWaveSpeed = 0.05f;
    constexpr float kTitleWaveAmplitude = 10.0f;
    constexpr float kTitlePositionX = 20.0f;
    constexpr float kTitlePositionY = 20.0f;
}

TitleScene::~TitleScene() = default;

void TitleScene::Initialize() {
    input_ = Input::GetInstance();
    dxCommon_ = DirectXCommon::GetInstance();

    normalButtonSize_ = kNormalButtonSize;
    hoverButtonSize_ = kHoverButtonSize;
    startButtonPosition_ = kStartButtonPosition;
    stageSelectButtonPosition_ = kStageSelectButtonPosition;

    titleTextureHandle_ = TextureManager::Load("title/GameTitle.png");
    titleSprite_.reset(Sprite::Create(titleTextureHandle_, {kTitlePositionX, kTitlePositionY}));

    backgroundTextureHandle_ = TextureManager::Load("title/BackGround.png");
    backgroundSprite_.reset(Sprite::Create(backgroundTextureHandle_, {0.0f, 0.0f}));

    startButtonTextureHandle_ = TextureManager::Load("title/StartButton.png");
    startButtonSprite_.reset(Sprite::Create(startButtonTextureHandle_, startButtonPosition_));
    if (startButtonSprite_) {
        startButtonSprite_->SetSize(normalButtonSize_);
    }

    stageSelectButtonTextureHandle_ = TextureManager::Load("title/StageSelectButton.png");
    stageSelectButtonSprite_.reset(Sprite::Create(stageSelectButtonTextureHandle_, stageSelectButtonPosition_));
    if (stageSelectButtonSprite_) {
        stageSelectButtonSprite_->SetSize(normalButtonSize_);
    }

    isSceneEnd_ = false;
    selectedLevel_ = kDefaultSelectedLevel;
    nextSceneState_ = LOADING;
    frameCount_ = 0;
    isMouseOverStart_ = false;
    isMouseOverStageSelect_ = false;
}

void TitleScene::Update() {
    frameCount_++;

    float offsetY = std::sin(frameCount_ * kTitleWaveSpeed) * kTitleWaveAmplitude;
    if (titleSprite_) {
        titleSprite_->SetPosition({kTitlePositionX, kTitlePositionY + offsetY});
    }

    UpdateButtonStates();

    Vector2 mousePos = input_->GetMousePosition();

    if (input_->IsTriggerMouse(kLeftMouseButton)) {
        if (IsMouseOverButton(mousePos, startButtonSprite_.get())) {
            selectedLevel_ = kDefaultSelectedLevel;
            nextSceneState_ = LOADING;
            isSceneEnd_ = true;
        }
        else if (IsMouseOverButton(mousePos, stageSelectButtonSprite_.get())) {
            selectedLevel_ = kStageSelectSelectedLevel;
            nextSceneState_ = STAGE_SELECT;
            isSceneEnd_ = true;
        }
    }
}

void TitleScene::UpdateButtonStates() {
    Vector2 mousePos = input_->GetMousePosition();

    bool wasMouseOverStart = isMouseOverStart_;
    bool wasMouseOverStageSelect = isMouseOverStageSelect_;

    isMouseOverStart_ = IsMouseOverButton(mousePos, startButtonSprite_.get());
    isMouseOverStageSelect_ = IsMouseOverButton(mousePos, stageSelectButtonSprite_.get());

    ButtonUtility::UpdateHoverSprite(
        startButtonSprite_.get(),
        startButtonPosition_,
        normalButtonSize_,
        hoverButtonSize_,
        isMouseOverStart_,
        wasMouseOverStart
    );

    ButtonUtility::UpdateHoverSprite(
        stageSelectButtonSprite_.get(),
        stageSelectButtonPosition_,
        normalButtonSize_,
        hoverButtonSize_,
        isMouseOverStageSelect_,
        wasMouseOverStageSelect
    );
}

void TitleScene::Draw() {
    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
    Sprite::PreDraw(commandList);

    if (backgroundSprite_) {
        backgroundSprite_->Draw();
    }
    if (titleSprite_) {
        titleSprite_->Draw();
    }
    if (startButtonSprite_) {
        startButtonSprite_->Draw();
    }
    if (stageSelectButtonSprite_) {
        stageSelectButtonSprite_->Draw();
    }

    Sprite::PostDraw();
    dxCommon_->ClearDepthBuffer();
}

bool TitleScene::IsMouseOverButton(const Vector2& mousePos, Sprite* buttonSprite) {
    if (!buttonSprite) {
        return false;
    }

    Vector2 position = buttonSprite->GetPosition();
    Vector2 size = buttonSprite->GetSize();

    return (mousePos.x >= position.x && mousePos.x <= position.x + size.x &&
        mousePos.y >= position.y && mousePos.y <= position.y + size.y);
}
