#include "Level1.h"

void Level1::Initialize() {
	camera_.Initialize();
	input_ = KamataEngine::Input::GetInstance();

	// 设置两个球的初始位置
	const int ballCount = 2;
	for (int i = 0; i < ballCount; i++) {
		Ball* ball = new Ball();
		ball->Initialize(&camera_);
		KamataEngine::Vector3 position = {-30.0f + i * 5.0f, 0.0f, 0.0f};
		ball->SetPosition(position);
		balls_.push_back(ball);
	}

	explosionTriggered_ = false;
	levelState_ = LevelState::PLAYING;
}

void Level1::Update() {
	// 更新所有球体
	for (Ball* ball : balls_) {
		ball->Update();
	}

	camera_.UpdateMatrix();

	// 检测K键按下触发爆炸
	if (input_->TriggerKey(DIK_K) && !explosionTriggered_) {
		explosionTriggered_ = true;

		// 引爆第一个球
		if (!balls_.empty()) {
			balls_[0]->Explode();
			TriggerExplosion(balls_[0]->GetPosition());
		}
	}

	// 检查关卡完成条件：第二个球被推动
	if (explosionTriggered_ && balls_.size() >= 2) {
		Ball* secondBall = balls_[1];
		KamataEngine::Vector3 pos = secondBall->GetPosition();
		if (pos.x > -28.0f || std::abs(pos.y) > 0.5f) {
			levelState_ = LevelState::COMPLETED;
		}
	}
}

void Level1::Draw() {
	for (Ball* ball : balls_) {
		ball->Draw();
	}
}

void Level1::OnEnter() {
	// 可以在这里播放关卡开始的音效或显示提示信息
}