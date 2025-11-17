#include "Stage.h"
using namespace KamataEngine;
Stage::~Stage() {
	delete bgSprite1_;
	delete bgSprite2_;
}
void Stage::Initialize() {
	textureHandle_ = TextureManager::Load("stage.png");
	bgSprite1_ = Sprite::Create(textureHandle_, {0, 0});
	bgSprite2_ = Sprite::Create(textureHandle_, {1280, 0});
}

void Stage::Update() {
	//scrollX_ -= scrollSpeed_;

	//if (scrollX_ <= -screenWidth_) {
	//	scrollX_ += screenWidth_;
	//}

	//// 2枚のスプライトの位置を設定
	//bgSprite1_->SetPosition({scrollX_, 0});
	//bgSprite2_->SetPosition({scrollX_ + screenWidth_, 0});
}

void Stage::Draw() {
	bgSprite1_->Draw();
	bgSprite2_->Draw();
}