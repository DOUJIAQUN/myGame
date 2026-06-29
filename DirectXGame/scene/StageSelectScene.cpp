#include "StageSelectScene.h"
#include "ButtonUtility.h"
#include "../DebugLogger.h" 
using namespace KamataEngine;

StageSelectScene::StageSelectScene() {}

StageSelectScene::~StageSelectScene() {
    delete backgroundSprite_;
    delete stage1ButtonSprite_;
    delete stage2ButtonSprite_;
    if (gameUI_) {
        delete gameUI_;
    }
}

void StageSelectScene::Initialize() {
    input_ = Input::GetInstance();
    dxCommon_ = DirectXCommon::GetInstance();
  
     gameUI_ = new GameUI();  
     if (gameUI_) {
         gameUI_->Initialize();
         // 设置只显示 title 按钮，隐藏 restart 按钮
         gameUI_->SetShowRestartButton(false);
         gameUI_->SetShowTitleButton(true);
     }
   
    // 加载背景
    backgroundTextureHandle_ = TextureManager::Load("stageSelect/BackGround.png");
    backgroundSprite_ = Sprite::Create(backgroundTextureHandle_, { 0, 0 });

    // 加载关卡按钮
    stage1ButtonTextureHandle_ = TextureManager::Load("stageSelect/stage1.png");
    stage1ButtonSprite_ = Sprite::Create(stage1ButtonTextureHandle_, stage1ButtonPosition_);
    if (stage1ButtonSprite_) {
        stage1ButtonSprite_->SetSize(normalButtonSize_);
    }

    stage2ButtonTextureHandle_ = TextureManager::Load("stageSelect/stage2.png");
    stage2ButtonSprite_ = Sprite::Create(stage2ButtonTextureHandle_, stage2ButtonPosition_);
    if (stage2ButtonSprite_) {
        stage2ButtonSprite_->SetSize(normalButtonSize_);
    }

  
    isSceneEnd_ = false;
    selectedLevel_ = 0;

    isMouseOverStage1_ = false;
    isMouseOverStage2_ = false;
}

void StageSelectScene::Update() {
    Vector2 mousePos = input_->GetMousePosition();

    UpdateButtonStates();

    if (gameUI_) {
        gameUI_->Update();
    }
    if (gameUI_ && gameUI_->IsReturnToTitleClicked()) {
        selectedLevel_ = 0;
        nextSceneState_ = TITLE;
        isSceneEnd_ = true;
        return;
    }

    if (input_->IsTriggerMouse(0)) {
        if (IsMouseOverButton(mousePos, stage1ButtonSprite_)) {
            selectedLevel_ = 1;
            nextSceneState_ = LOADING;
            isSceneEnd_ = true;
        }
        else if (IsMouseOverButton(mousePos, stage2ButtonSprite_)) {
            selectedLevel_ = 5;
            nextSceneState_ = LOADING;
            isSceneEnd_ = true;
        }
    }
}

//更新按钮状态方法
void StageSelectScene::UpdateButtonStates() {
    // 获取鼠标位置
    Vector2 mousePos = input_->GetMousePosition();

    // 检测鼠标是否在按钮上
    bool wasMouseOverStage1 = isMouseOverStage1_;
    bool wasMouseOverStage2 = isMouseOverStage2_;

    isMouseOverStage1_ = IsMouseOverButton(mousePos, stage1ButtonSprite_);
    isMouseOverStage2_ = IsMouseOverButton(mousePos, stage2ButtonSprite_);

    // 如果Stage1按钮状态发生变化，更新按钮尺寸
    ButtonUtility::UpdateHoverSprite(
        stage1ButtonSprite_,
        stage1ButtonPosition_,
        normalButtonSize_,
        hoverButtonSize_,
        isMouseOverStage1_,
        wasMouseOverStage1
    );

    // 如果Stage2按钮状态发生变化，更新按钮尺寸
    ButtonUtility::UpdateHoverSprite(
        stage2ButtonSprite_,
        stage2ButtonPosition_,
        normalButtonSize_,
        hoverButtonSize_,
        isMouseOverStage2_,
        wasMouseOverStage2
    );
}
void StageSelectScene::Draw() {
    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
    Sprite::PreDraw(commandList);

    backgroundSprite_->Draw();

    if (stage1ButtonSprite_) stage1ButtonSprite_->Draw();
    if (stage2ButtonSprite_) stage2ButtonSprite_->Draw();
    // 绘制 UI（返回按钮）
    if (gameUI_) {
        gameUI_->Draw();
    }

    Sprite::PostDraw();
    dxCommon_->ClearDepthBuffer();
}

bool StageSelectScene::IsMouseOverButton(const Vector2& mousePos, Sprite* buttonSprite) {
    if (!buttonSprite) return false;

    Vector2 position = buttonSprite->GetPosition();
    Vector2 size = buttonSprite->GetSize();

    return (mousePos.x >= position.x && mousePos.x <= position.x + size.x &&
        mousePos.y >= position.y && mousePos.y <= position.y + size.y);
}