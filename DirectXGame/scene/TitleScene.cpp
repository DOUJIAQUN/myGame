#include "TitleScene.h"
#include "ButtonUtility.h"

using namespace KamataEngine;
TitleScene::~TitleScene()
{
    delete titleSprite_;
    delete backgroundSprite_;
    delete startButtonSprite_;
    delete stageSelectButtonSprite_;
}
void TitleScene::Initialize() {
    input_ = Input::GetInstance();
    dxCommon_ = DirectXCommon::GetInstance();

    titleTextureHandle_ = TextureManager::Load("title/GameTitle.png");
    titleSprite_ = Sprite::Create(titleTextureHandle_, {0, 0});
	backgroundTextureHandle_ = TextureManager::Load("title/BackGround.png");
	backgroundSprite_ = Sprite::Create(backgroundTextureHandle_, {0, 0});

    // 加载按钮纹理
    startButtonTextureHandle_ = TextureManager::Load("title/StartButton.png");
    startButtonSprite_ = Sprite::Create(startButtonTextureHandle_, startButtonPosition_);
    if (startButtonSprite_) {
        startButtonSprite_->SetSize(normalButtonSize_);
    }

    stageSelectButtonTextureHandle_ = TextureManager::Load("title/StageSelectButton.png");
    stageSelectButtonSprite_ = Sprite::Create(stageSelectButtonTextureHandle_, stageSelectButtonPosition_);
    if (stageSelectButtonSprite_) {
        stageSelectButtonSprite_->SetSize(normalButtonSize_);
    }

    isSceneEnd_ = false;
    selectedLevel_ = 1; // 默认直接开始第一关
    frameCount_ = 0;
    isMouseOverStart_ = false;
    isMouseOverStageSelect_ = false;
}

void TitleScene::Update() {
    frameCount_++;

    float offsetY = std::sin(frameCount_ * 0.05f) * 10.0f;
    titleSprite_->SetPosition({ 20, 20 + offsetY });

    UpdateButtonStates();

    Vector2 mousePos = input_->GetMousePosition();

    if (input_->IsTriggerMouse(0)) {
        if (IsMouseOverButton(mousePos, startButtonSprite_)) {
            selectedLevel_ = 1;
            nextSceneState_ = LOADING;   // 直接开始游戏
            isSceneEnd_ = true;
        }
        else if (IsMouseOverButton(mousePos, stageSelectButtonSprite_)) {
            selectedLevel_ = 0;
            nextSceneState_ = STAGE_SELECT; // 进入关卡选择
            isSceneEnd_ = true;
        }
    }
}

// 新增：更新按钮状态方法
void TitleScene::UpdateButtonStates() {
    // 获取鼠标位置
    Vector2 mousePos = input_->GetMousePosition();

    // 检测鼠标是否在按钮上
    bool wasMouseOverStart = isMouseOverStart_;
    bool wasMouseOverStageSelect = isMouseOverStageSelect_;

    isMouseOverStart_ = IsMouseOverButton(mousePos, startButtonSprite_);
    isMouseOverStageSelect_ = IsMouseOverButton(mousePos, stageSelectButtonSprite_);

    // 如果Start按钮状态发生变化，更新按钮尺寸
    ButtonUtility::UpdateHoverSprite(
    startButtonSprite_,
    startButtonPosition_,
    normalButtonSize_,
    hoverButtonSize_,
    isMouseOverStart_,
    wasMouseOverStart
);

    // 如果StageSelect按钮状态发生变化，更新按钮尺寸
    ButtonUtility::UpdateHoverSprite(
        stageSelectButtonSprite_,
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
    // 绘制背景
	backgroundSprite_->Draw();
    // 绘制标题
    titleSprite_->Draw();
    // 绘制按钮
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
    if (!buttonSprite) return false;

    // 使用当前实际的精灵尺寸和位置进行检测
    Vector2 position = buttonSprite->GetPosition();
    Vector2 size = buttonSprite->GetSize();

    return (mousePos.x >= position.x && mousePos.x <= position.x + size.x &&
        mousePos.y >= position.y && mousePos.y <= position.y + size.y);
}


