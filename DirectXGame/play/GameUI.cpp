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
            restartSprite_->SetSize(buttonSize_);
        }
    }

    // 加载返回标题按钮图标
    titleTextureHandle_ = TextureManager::Load("ui/title.png");
    if (titleTextureHandle_ != 0) {
        titleSprite_ = Sprite::Create(titleTextureHandle_, titlePosition_);
        if (titleSprite_) {
            titleSprite_->SetSize(buttonSize_);
        }
    }
}

void GameUI::Update() {
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

void GameUI::Draw() {
    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

    // 在前景层绘制UI
    if (restartSprite_) {
        restartSprite_->Draw();
    }
    if (titleSprite_) {
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