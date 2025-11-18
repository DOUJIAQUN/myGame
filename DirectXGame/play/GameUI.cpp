#include "GameUI.h"
#include <cassert>

GameUI::GameUI() {}

GameUI::~GameUI() {
    if (restartSprite_) {
        delete restartSprite_;
    }
    if (titleSprite_) {
        delete titleSprite_;
    }
}

void GameUI::Initialize() {
    input_ = Input::GetInstance();
    dxCommon_ = DirectXCommon::GetInstance();

    isRestartClicked_ = false;
    isReturnToTitleClicked_ = false;

    LoadTextures();
}

void GameUI::LoadTextures() {
    // 加载重新开始按钮图标
    restartTextureHandle_ = TextureManager::Load("ui/restart.png");
    if (restartTextureHandle_ != 0) {
        restartSprite_ = Sprite::Create(restartTextureHandle_, restartPosition_);
        if (restartSprite_) {
            restartSprite_->SetSize(normalButtonSize_);
        }
    }

    // 加载返回标题按钮图标
    titleTextureHandle_ = TextureManager::Load("ui/title.png");
    if (titleTextureHandle_ != 0) {
        titleSprite_ = Sprite::Create(titleTextureHandle_, titlePosition_);
        if (titleSprite_) {
            titleSprite_->SetSize(normalButtonSize_);
        }
    }
}

void GameUI::Update() {

    // 更新按钮状态（检测鼠标悬停）
    UpdateButtonStates();

    // 重置点击状态
    isRestartClicked_ = false;
    isReturnToTitleClicked_ = false;

    // 获取鼠标位置
    Vector2 mousePos = input_->GetMousePosition();

    // 检测鼠标点击
    if (input_->IsTriggerMouse(0)) {
        if (IsMouseOverRestart(mousePos)) {
            isRestartClicked_ = true;
        }
        else if (IsMouseOverTitle(mousePos)) {
            isReturnToTitleClicked_ = true;
        }
    }
}
// 新增：更新按钮状态方法
void GameUI::UpdateButtonStates() {
    // 获取鼠标位置
    Vector2 mousePos = input_->GetMousePosition();

    // 检测鼠标是否在按钮上
    bool wasMouseOverRestart = isMouseOverRestart_;
    bool wasMouseOverTitle = isMouseOverTitle_;

    isMouseOverRestart_ = IsMouseOverRestart(mousePos);
    isMouseOverTitle_ = IsMouseOverTitle(mousePos);

    // 如果Restart按钮状态发生变化，更新按钮尺寸
    if (isMouseOverRestart_ != wasMouseOverRestart && restartSprite_) {
        if (isMouseOverRestart_) {
            // 鼠标进入：放大按钮
            restartSprite_->SetSize(hoverButtonSize_);

            // 调整位置保持中心点不变
            Vector2 sizeDiff = {
                (hoverButtonSize_.x - normalButtonSize_.x) / 2,
                (hoverButtonSize_.y - normalButtonSize_.y) / 2
            };
            restartSprite_->SetPosition({
                restartPosition_.x - sizeDiff.x,
                restartPosition_.y - sizeDiff.y
                });
        }
        else {
            // 鼠标离开：恢复正常尺寸
            restartSprite_->SetSize(normalButtonSize_);
            restartSprite_->SetPosition(restartPosition_);
        }
    }

    // 如果Title按钮状态发生变化，更新按钮尺寸
    if (isMouseOverTitle_ != wasMouseOverTitle && titleSprite_) {
        if (isMouseOverTitle_) {
            // 鼠标进入：放大按钮
            titleSprite_->SetSize(hoverButtonSize_);

            // 调整位置保持中心点不变
            Vector2 sizeDiff = {
                (hoverButtonSize_.x - normalButtonSize_.x) / 2,
                (hoverButtonSize_.y - normalButtonSize_.y) / 2
            };
            titleSprite_->SetPosition({
                titlePosition_.x - sizeDiff.x,
                titlePosition_.y - sizeDiff.y
                });
        }
        else {
            // 鼠标离开：恢复正常尺寸
            titleSprite_->SetSize(normalButtonSize_);
            titleSprite_->SetPosition(titlePosition_);
        }
    }
}
void GameUI::Draw() {
    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

    // 在前景层绘制UI（根据显示状态）
    if (restartSprite_ && showRestartButton_) {
        restartSprite_->Draw();
    }
    if (titleSprite_ && showTitleButton_) {
        titleSprite_->Draw();
    }
}

bool GameUI::IsMouseOverRestart(const Vector2& mousePos) {
    if (!restartSprite_) return false;

    Vector2 position = restartSprite_->GetPosition();
    Vector2 size = restartSprite_->GetSize();

    return (mousePos.x >= position.x && mousePos.x <= position.x + size.x &&
        mousePos.y >= position.y && mousePos.y <= position.y + size.y);
}

bool GameUI::IsMouseOverTitle(const Vector2& mousePos) {
    if (!titleSprite_) return false;

    Vector2 position = titleSprite_->GetPosition();
    Vector2 size = titleSprite_->GetSize();

    return (mousePos.x >= position.x && mousePos.x <= position.x + size.x &&
        mousePos.y >= position.y && mousePos.y <= position.y + size.y);
}

void GameUI::ResetClicks() {
    isRestartClicked_ = false;
    isReturnToTitleClicked_ = false;
}