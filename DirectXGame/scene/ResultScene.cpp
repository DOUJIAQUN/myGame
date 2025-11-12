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

    LoadTextures();
}

void ResultScene::LoadTextures() {
    // 加载结果背景图片
    resultTextureHandle_ = TextureManager::Load("result/resultBackground.png");
   
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
        titleSprite_ = Sprite::Create(titleTextureHandle_, { 440, 520 }); // 按钮位置
        if (titleSprite_) {
            titleSprite_->SetSize({ 400, 100 }); // 按钮大小
        }
    }
}

void ResultScene::Update() {
    if (isSceneEnd_) return;

    // 获取鼠标位置
    Vector2 mousePos = input_->GetMousePosition();

    // 检测鼠标点击
    if (input_->IsTriggerMouse(0)) {
        
        if (IsMouseOverTitle(mousePos)) {
            // 返回标题
            isSceneEnd_ = true;
           
        }
        
      
    }
}

void ResultScene::Draw() {
    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
    Sprite::PreDraw(commandList);
    // 绘制结果背景
  
    resultSprite_->Draw();
    

  
    if (titleSprite_) {
        titleSprite_->Draw();
    }
    Sprite::PostDraw();
    dxCommon_->ClearDepthBuffer();
}
 



bool ResultScene::IsMouseOverTitle(const Vector2& mousePos) {
    if (!titleSprite_) return false;

    Vector2 position = titleSprite_->GetPosition();
    Vector2 size = titleSprite_->GetSize();

    return (mousePos.x >= position.x && mousePos.x <= position.x + size.x &&
        mousePos.y >= position.y && mousePos.y <= position.y + size.y);
}