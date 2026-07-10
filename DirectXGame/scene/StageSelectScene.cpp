#include "StageSelectScene.h"
#include "ButtonUtility.h"
#include "../DebugLogger.h"

using namespace KamataEngine;

namespace {
    const Vector2 kNormalButtonSize = {200.0f, 100.0f};
    const Vector2 kHoverButtonSize = {220.0f, 110.0f};
    const Vector2 kStage1ButtonPosition = {340.0f, 300.0f};
    const Vector2 kStage2ButtonPosition = {740.0f, 300.0f};
    constexpr int kLeftMouseButton = 0;
    constexpr int kReturnTitleSelectedLevel = 0;
    constexpr int kStage1StartLevel = 1;
    constexpr int kStage2StartLevel = 5;
}

StageSelectScene::StageSelectScene() = default;
StageSelectScene::~StageSelectScene() = default;

// 関数コメント: Initialize の処理を実行する。
void StageSelectScene::Initialize() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Initialize の役割を実現する。
    input_ = Input::GetInstance();
    dxCommon_ = DirectXCommon::GetInstance();

    normalButtonSize_ = kNormalButtonSize;
    hoverButtonSize_ = kHoverButtonSize;
    stage1ButtonPosition_ = kStage1ButtonPosition;
    stage2ButtonPosition_ = kStage2ButtonPosition;

    gameUI_ = std::make_unique<GameUI>();
    gameUI_->Initialize();
    gameUI_->SetShowRestartButton(false);
    gameUI_->SetShowTitleButton(true);

    backgroundTextureHandle_ = TextureManager::Load("stageSelect/BackGround.png");
    backgroundSprite_.reset(Sprite::Create(backgroundTextureHandle_, {0.0f, 0.0f}));

    stage1ButtonTextureHandle_ = TextureManager::Load("stageSelect/stage1.png");
    stage1ButtonSprite_.reset(Sprite::Create(stage1ButtonTextureHandle_, stage1ButtonPosition_));
    if (stage1ButtonSprite_) {
        stage1ButtonSprite_->SetSize(normalButtonSize_);
    }

    stage2ButtonTextureHandle_ = TextureManager::Load("stageSelect/stage2.png");
    stage2ButtonSprite_.reset(Sprite::Create(stage2ButtonTextureHandle_, stage2ButtonPosition_));
    if (stage2ButtonSprite_) {
        stage2ButtonSprite_->SetSize(normalButtonSize_);
    }

    isSceneEnd_ = false;
    selectedLevel_ = kReturnTitleSelectedLevel;
    nextSceneState_ = TITLE;

    isMouseOverStage1_ = false;
    isMouseOverStage2_ = false;
}

// 関数コメント: Update の処理を実行する。
void StageSelectScene::Update() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Update の役割を実現する。
    Vector2 mousePos = input_->GetMousePosition();

    UpdateButtonStates();

    if (gameUI_) {
        gameUI_->Update();
    }
    if (gameUI_ && gameUI_->IsReturnToTitleClicked()) {
        selectedLevel_ = kReturnTitleSelectedLevel;
        nextSceneState_ = TITLE;
        isSceneEnd_ = true;
        return;
    }

    if (input_->IsTriggerMouse(kLeftMouseButton)) {
        if (IsMouseOverButton(mousePos, stage1ButtonSprite_.get())) {
            selectedLevel_ = kStage1StartLevel;
            nextSceneState_ = LOADING;
            isSceneEnd_ = true;
        }
        // 関数コメント: if の処理を実行する。
        else if (IsMouseOverButton(mousePos, stage2ButtonSprite_.get())) {
            // 処理コメント: 必要な状態確認やデータ更新を行い、if の役割を実現する。
            selectedLevel_ = kStage2StartLevel;
            nextSceneState_ = LOADING;
            isSceneEnd_ = true;
        }
    }
}

// 関数コメント: UpdateButtonStates の処理を実行する。
void StageSelectScene::UpdateButtonStates() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、UpdateButtonStates の役割を実現する。
    Vector2 mousePos = input_->GetMousePosition();

    bool wasMouseOverStage1 = isMouseOverStage1_;
    bool wasMouseOverStage2 = isMouseOverStage2_;

    isMouseOverStage1_ = IsMouseOverButton(mousePos, stage1ButtonSprite_.get());
    isMouseOverStage2_ = IsMouseOverButton(mousePos, stage2ButtonSprite_.get());

    ButtonUtility::UpdateHoverSprite(
        stage1ButtonSprite_.get(),
        stage1ButtonPosition_,
        normalButtonSize_,
        hoverButtonSize_,
        isMouseOverStage1_,
        wasMouseOverStage1
    );

    ButtonUtility::UpdateHoverSprite(
        stage2ButtonSprite_.get(),
        stage2ButtonPosition_,
        normalButtonSize_,
        hoverButtonSize_,
        isMouseOverStage2_,
        wasMouseOverStage2
    );
}

// 関数コメント: Draw の処理を実行する。
void StageSelectScene::Draw() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Draw の役割を実現する。
    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
    Sprite::PreDraw(commandList);

    if (backgroundSprite_) {
        backgroundSprite_->Draw();
    }
    if (stage1ButtonSprite_) {
        stage1ButtonSprite_->Draw();
    }
    if (stage2ButtonSprite_) {
        stage2ButtonSprite_->Draw();
    }
    if (gameUI_) {
        gameUI_->Draw();
    }

    Sprite::PostDraw();
    dxCommon_->ClearDepthBuffer();
}

// 関数コメント: IsMouseOverButton の処理を実行する。
bool StageSelectScene::IsMouseOverButton(const Vector2& mousePos, Sprite* buttonSprite) {
    // 処理コメント: 必要な状態確認やデータ更新を行い、IsMouseOverButton の役割を実現する。
    if (!buttonSprite) {
        return false;
    }

    Vector2 position = buttonSprite->GetPosition();
    Vector2 size = buttonSprite->GetSize();

    return (mousePos.x >= position.x && mousePos.x <= position.x + size.x &&
        mousePos.y >= position.y && mousePos.y <= position.y + size.y);
}
