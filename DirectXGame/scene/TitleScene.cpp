#include "TitleScene.h"
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

    // 上下に揺らす（sin波でY座標を変更）
    float offsetY = std::sin(frameCount_ * 0.05f) * 10.0f;
    titleSprite_->SetPosition({ 20, 20 + offsetY });

    // 更新按钮状态（检测鼠标悬停）
    UpdateButtonStates();

    // 获取鼠标位置
    Vector2 mousePos = input_->GetMousePosition();

    if (input_->IsTriggerMouse(0)) {
        if (IsMouseOverButton(mousePos, startButtonSprite_)) {
            // 开始游戏（直接第一关）
            selectedLevel_ = 1;
            isSceneEnd_ = true;
        }
        else if (IsMouseOverButton(mousePos, stageSelectButtonSprite_)) {
            // 进入关卡选择界面
            selectedLevel_ = 0; // 0 表示进入关卡选择
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
    if (isMouseOverStart_ != wasMouseOverStart && startButtonSprite_) {
        if (isMouseOverStart_) {
            // 鼠标进入：放大按钮
            startButtonSprite_->SetSize(hoverButtonSize_);

            // 调整位置保持中心点不变
            Vector2 sizeDiff = {
                (hoverButtonSize_.x - normalButtonSize_.x) / 2,
                (hoverButtonSize_.y - normalButtonSize_.y) / 2
            };
            startButtonSprite_->SetPosition({
                startButtonPosition_.x - sizeDiff.x,
                startButtonPosition_.y - sizeDiff.y
                });
        }
        else {
            // 鼠标离开：恢复正常尺寸
            startButtonSprite_->SetSize(normalButtonSize_);
            startButtonSprite_->SetPosition(startButtonPosition_);
        }
    }

    // 如果StageSelect按钮状态发生变化，更新按钮尺寸
    if (isMouseOverStageSelect_ != wasMouseOverStageSelect && stageSelectButtonSprite_) {
        if (isMouseOverStageSelect_) {
            // 鼠标进入：放大按钮
            stageSelectButtonSprite_->SetSize(hoverButtonSize_);

            // 调整位置保持中心点不变
            Vector2 sizeDiff = {
                (hoverButtonSize_.x - normalButtonSize_.x) / 2,
                (hoverButtonSize_.y - normalButtonSize_.y) / 2
            };
            stageSelectButtonSprite_->SetPosition({
                stageSelectButtonPosition_.x - sizeDiff.x,
                stageSelectButtonPosition_.y - sizeDiff.y
                });
        }
        else {
            // 鼠标离开：恢复正常尺寸
            stageSelectButtonSprite_->SetSize(normalButtonSize_);
            stageSelectButtonSprite_->SetPosition(stageSelectButtonPosition_);
        }
    }
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

bool TitleScene::IsSceneEnd() {
    return isSceneEnd_;
}
