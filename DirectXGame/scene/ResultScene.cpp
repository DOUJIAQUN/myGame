#include "ResultScene.h"
#include <cassert>

using namespace KamataEngine;

ResultScene::ResultScene() {}

ResultScene::~ResultScene() {
    if (resultSprite_) {
        delete resultSprite_;
    }
    if (retrySprite_) {
        delete retrySprite_;
    }
    if (titleSprite_) {
        delete titleSprite_;
    }
}

void ResultScene::Initialize() {
    input_ = Input::GetInstance();
    dxCommon_ = DirectXCommon::GetInstance();

    isSceneEnd_ = false;
    score_ = 0;
    isMouseOverTitle_ = false;

    LoadTextures();
}

void ResultScene::LoadTextures() {
    // 加载结果背景图片
    resultBackgroundTextureHandle_ = TextureManager::Load("result/resultBackground.png");
   
    resultBackgroundSprite_ = Sprite::Create(resultBackgroundTextureHandle_, { 0, 0 });
   

    resultTextureHandle_ = TextureManager::Load("result/resultTitle.png");
    resultSprite_ = Sprite::Create(resultTextureHandle_, { 0, 0 });

    //// 加载重新开始按钮图片
    //retryTextureHandle_ = TextureManager::Load("result/retryButton.png");
    //if (retryTextureHandle_ != 0) {
    //    retrySprite_ = Sprite::Create(retryTextureHandle_, { 440, 400 }); // 按钮位置
    //    if (retrySprite_) {
    //        retrySprite_->SetSize({ 400, 100 }); // 按钮大小
    //    }
    //}

    // 加载返回标题按钮图片
    titleTextureHandle_ = TextureManager::Load("result/titleButton.png");
    if (titleTextureHandle_ != 0) {
        titleSprite_ = Sprite::Create(titleTextureHandle_,  buttonPosition_ ); // 按钮位置
        if (titleSprite_) {
            titleSprite_->SetSize(normalButtonSize_); // 按钮大小
        }
    }
}

void ResultScene::Update() {
    if (isSceneEnd_) return;

    frameCount_++;

    // 上下に揺らす（sin波でY座標を変更）
    float offsetY = std::sin(frameCount_ * 0.05f) * 10.0f;
    resultSprite_->SetPosition({ 20, 20 + offsetY });
  

    UpdateButtonState();

    // 检测鼠标点击
    if (input_->IsTriggerMouse(0)) {
        
        if (isMouseOverTitle_) {
            // 返回标题
            isSceneEnd_ = true;
           
        }
        
      
    }
}


// 新增：更新按钮状态方法
void ResultScene::UpdateButtonState() {
    // 获取鼠标位置
    Vector2 mousePos = input_->GetMousePosition();

    // 检测鼠标是否在按钮上
    bool wasMouseOver = isMouseOverTitle_;
    isMouseOverTitle_ = IsMouseOverTitle(mousePos);

    // 如果状态发生变化，更新按钮尺寸
    if (isMouseOverTitle_ != wasMouseOver && titleSprite_) {
        if (isMouseOverTitle_) {
            // 鼠标进入：放大按钮
            titleSprite_->SetSize(hoverButtonSize_);

            // 调整位置保持中心点不变（可选）
            Vector2 sizeDiff = {
                (hoverButtonSize_.x - normalButtonSize_.x) / 2,
                (hoverButtonSize_.y - normalButtonSize_.y) / 2
            };
            titleSprite_->SetPosition({
                buttonPosition_.x - sizeDiff.x,
                buttonPosition_.y - sizeDiff.y
                });
        }
        else {
            // 鼠标离开：恢复正常尺寸
            titleSprite_->SetSize(normalButtonSize_);
            titleSprite_->SetPosition(buttonPosition_);
        }
    }
}

void ResultScene::Draw() {
    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
    Sprite::PreDraw(commandList);
    // 绘制结果背景
    resultBackgroundSprite_->Draw();

    resultSprite_->Draw();
    

  
    if (titleSprite_) {
        titleSprite_->Draw();
    }
    Sprite::PostDraw();
    dxCommon_->ClearDepthBuffer();
}
 



bool ResultScene::IsMouseOverTitle(const Vector2& mousePos) {
    if (!titleSprite_) return false;

    // 使用当前实际的精灵尺寸和位置进行检测
    Vector2 position = titleSprite_->GetPosition();
    Vector2 size = titleSprite_->GetSize();

    return (mousePos.x >= position.x && mousePos.x <= position.x + size.x &&
        mousePos.y >= position.y && mousePos.y <= position.y + size.y);
}