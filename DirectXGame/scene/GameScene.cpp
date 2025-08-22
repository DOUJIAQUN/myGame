#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete stage_;
	for (Ball* ball : balls_) {
		delete ball;
	}
	balls_.clear(); // 清空向量
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	camera_.Initialize();

	stage_ = new Stage();
	stage_->Initialize();

	  // 创建两个 Ball 实例并设置不同位置
    const int ballCount = 2;
    for (int i = 0; i < ballCount; i++) {
        Ball* ball = new Ball();
        ball->Initialize(&camera_);
        
        // 使用 SetPosition 方法设置位置
        // 第一个球在 (-30, 0, 0)，第二个球在 (-25, 0, 0)
        KamataEngine::Vector3 position = {-30.0f + i * 5.0f, 0.0f, 0.0f};
        ball->SetPosition(position);
        
        balls_.push_back(ball);
    }
}



void GameScene::Update() {
	stage_->Update();
	// 更新所有 Ball
	for (Ball* ball : balls_) {
		ball->Update();
	}
    
	camera_.UpdateMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	stage_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (Ball* ball : balls_) {
		ball->Draw();
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}