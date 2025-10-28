#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete stage_;
	for (Ball* ball : balls_) {
		delete ball;
	}
	balls_.clear(); // 清空向量

	  // 释放教程相关的Sprite资源
	for (auto sprite : tutorialSprites_) {
		delete sprite;
	}
	tutorialSprites_.clear();

	if (startSprite_) {
		delete startSprite_;
		startSprite_ = nullptr;
	}

	// 清理UI
	if (gameUI_) {
		delete gameUI_;
		gameUI_ = nullptr;
	}

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	camera_.Initialize();

	stage_ = new Stage();
	stage_->Initialize();

	// 初始化游戏UI
	gameUI_ = new GameUI();
	gameUI_->Initialize();

	   // 初始化教程系统
	LoadTutorialTextures();           
	gameState_ = GameState::Tutorial;
	currentTutorialIndex_ = 0;
	showStart_ = false;
	startTimer_ = 0.0f;

	// 初始化游戏结束状态
	isGameOver_ = false;


	  // 创建两个 Ball 实例并设置不同位置
    const int ballCount = 4;
	KamataEngine::Vector3 positions[4] = {
	    {-30.0f, 0.0f, 0.0f}, // 第一个小球
	    {-20.0f, 0.0f, 0.0f}, // 第二个小球
	    { 0.0f, 0.0f, 0.0f}, // 第三个小球
	    {15.0f, 0.0f, 0.0f}  // 第四个小球
	};

	for (int i = 0; i < ballCount; i++) {
		Ball* ball = new Ball();
		ball->Initialize(&camera_);
		ball->SetInitialPosition(positions[i]);
		balls_.push_back(ball);
	}
	const int GoalCount = 1;
	KamataEngine::Vector3 goalPositions[1] = {
	    {25.0f, 0.0f, 0.0f},
	    
	};
	for (int i = 0; i < GoalCount; i++) {
		Goal* goal = new Goal();
		goal->Initialize(&camera_);
		goal->SetPosition(goalPositions[i]);
		goals_.push_back(goal);
	}
}


void GameScene::LoadTutorialTextures() {
	// 加载教程图片纹理并创建对应的Sprite
	tutorialTextureHandles_.push_back(TextureManager::Load("gameTutorial/tutorial1.png"));
	tutorialTextureHandles_.push_back(TextureManager::Load("gameTutorial/tutorial2.png"));
	tutorialTextureHandles_.push_back(TextureManager::Load("gameTutorial/tutorial3.png"));
	tutorialTextureHandles_.push_back(TextureManager::Load("gameTutorial/tutorial4.png"));

	// 为每个纹理创建Sprite对象（全屏显示）
	for (auto handle : tutorialTextureHandles_) {
		KamataEngine::Sprite* sprite = Sprite::Create(handle, {0, 0});
		tutorialSprites_.push_back(sprite);
	}

	// 加载开始图片
	startTextureHandle_ = TextureManager::Load("gameTutorial/gameStart.png");
	// 计算初始位置：让3倍大的图片居中显示
	// 屏幕中心坐标 - (图片宽度/2)
	float startX = 640.0f - (startSize_.x / 2.0f);
	float startY = 360.0f - (startSize_.y / 2.0f);
	startSprite_ = Sprite::Create(startTextureHandle_, {startX, startY});

	  // 设置初始尺寸（3倍大小）
	if (startSprite_) {
		startSprite_->SetSize(startSize_);
	}
}

void GameScene::Update() {

	switch (gameState_) {
	case GameState::Tutorial:
		UpdateTutorial();
		break;
	case GameState::StartWait:
		UpdateStartWait();
		break;
	case GameState::StartAnim: // 新增动画状态
		UpdateStartAnim();
		break;
	case GameState::Playing:


		stage_->Update();

		// 更新游戏UI
		if (gameUI_) {
			gameUI_->Update();

			// 检测重新开始
			if (gameUI_->IsRestartClicked()) {
				RestartLevel();
			}
			// 检测返回标题
			else if (gameUI_->IsReturnToTitleClicked()) {
				ReturnToTitle();
			}
		}


		// 获取鼠标位置
		mousePos = Input::GetInstance()->GetMousePosition();

		// 重置所有球的鼠标悬停状态
		for (Ball* ball : balls_) {
			ball->SetMouseOver(false);
		}

		// 检测鼠标悬停
		for (Ball* ball : balls_) {
			if (ball->IsActive() && !ball->IsExploded()) {
				if (IsMouseOverBall(ball, mousePos)) {
					ball->SetMouseOver(true);

					// 使用 GameScene 的 WorldToScreen 函数获取准确的屏幕坐标
					KamataEngine::Vector3 screenPos = WorldToScreen(ball->GetPosition());
					ball->UpdateExplosionRangePosition(screenPos);

					break; // 只处理第一个悬停的球体
				}
			}
		}


		if (input_->IsTriggerMouse(0)) {
			// 遍历所有球体，检查鼠标是否点击到球体
			for (size_t i = 0; i < balls_.size(); i++) {
				Ball* clickedBall = balls_[i];

				// 检查球体是否活跃且未被爆炸
				if (clickedBall->IsActive() && !clickedBall->IsExploded()) {
					// 使用IsMouseOverBall检测鼠标是否在球体范围内
					if (IsMouseOverBall(clickedBall, mousePos)) {
						// 这里处理鼠标点击到小球的情况
						clickedBall->Explode();

						// 获取爆炸位置
						KamataEngine::Vector3 explosionPos = clickedBall->GetPosition();
						const float explosionRadius = 10.1f;
						const float explosionForce = 1.0f;

						// 对范围内的其他球体施加爆炸力
						for (size_t j = 0; j < balls_.size(); j++) {
							if (i == j)
								continue; // 跳过被点击的球体本身

							Ball* otherBall = balls_[j];
							if (otherBall->IsActive()) {
								float distance = myMath::Distance(explosionPos, otherBall->GetPosition());

								if (distance <= explosionRadius) {
									// 计算爆炸方向（从爆炸中心指向球体）
									KamataEngine::Vector3 direction = myMath::Subtract(otherBall->GetPosition(), explosionPos);
									direction = myMath::Normalize(direction);

									KamataEngine::Vector3 force = myMath::Multiply(explosionForce, direction);

									// 应用爆炸力
									otherBall->ApplyExplosionForce(force);
								}
							}
						}
						break; // 只处理第一个被点击的球体
					}
				}
			}
		}
		// 更新所有 Ball
		for (Ball* ball : balls_) {
			ball->Update();
		}
		// 检测小球与终点的碰撞
		if (!isGameOver_) {
			for (Ball* ball : balls_) {
				for (Goal* goal : goals_) {
					if (ball->IsActive() && CheckBallGoalCollision(ball, goal)) {
						GameOver();
						break;
					}
				}
				if (isGameOver_) break; // 如果游戏结束，跳出循环
			}
		}
		break;
	case GameState::GameOver:
		// 游戏结束状态，等待一段时间或用户输入后结束场景
		// 这里可以添加计时器，或者检测按键输入
		
			isSceneEnd_ = true;
	
		break;
	}
	camera_.UpdateMatrix();
}

void GameScene::UpdateTutorial() {
	// 检测鼠标左键点击来切换教程图片
	if (input_->IsTriggerMouse(0)) {
		currentTutorialIndex_++;

		// 如果所有教程图片都播放完毕
		if (currentTutorialIndex_ >= tutorialSprites_.size()) {
			showStart_ = true;
			gameState_ = GameState::StartWait;
			startTimer_ = 0.0f;
		}
	}
}






void GameScene::UpdateStartWait() {
	// 更新计时器
	startTimer_ += 1.0f / 60.0f; // 假设60帧

  // 检测鼠标左键点击或等待1秒后开始动画
	if (input_->IsTriggerMouse(0)|| startTimer_ >= 0.01f) {
        // 进入动画状态
        gameState_ = GameState::StartAnim;
        animTimer_ = 0.0f;
        
        // 设置初始缩放（3倍大小）
        if (startSprite_) {
			// 重新设置初始位置和尺寸，确保一致性
			float startX = 640.0f - (startSize_.x / 2.0f);
			float startY = 360.0f - (startSize_.y / 2.0f);
			startSprite_->SetPosition({startX, startY});
			startSprite_->SetSize(startSize_);
        }
    }
}


void GameScene::UpdateStartAnim() {
	// 更新动画计时器
	animTimer_ += 1.0f / 60.0f;

	if (startSprite_) {
		// 计算动画进度（0.0 ~ 1.0）
		float progress = animTimer_ / animDuration_;
		if (progress > 1.0f)
			progress = 1.0f;

		// 使用缓动函数
		float easeProgress = 1.0f - (1.0f - progress) * (1.0f - progress) * (1.0f - progress); // easeOutCubic

		// 计算当前尺寸
		float currentSizeX = startSize_.x + (targetSize_.x - startSize_.x) * easeProgress;
		float currentSizeY = startSize_.y + (targetSize_.y - startSize_.y) * easeProgress;

		// 计算当前位置：让图片在缩放过程中保持居中
		float currentX = 640.0f - (currentSizeX / 2.0f);
		float currentY = 360.0f - (currentSizeY / 2.0f);

		// 应用尺寸和位置变化
		startSprite_->SetSize({currentSizeX, currentSizeY});
		startSprite_->SetPosition({currentX, currentY});

		// 动画结束后等待1秒再开始游戏
		if (progress >= 1.0f) {
			if (animTimer_ >= animDuration_ + displayDuration_) {
				StartGame();
			}
		}
	} else {
		// 如果没有开始图片，直接开始游戏
		StartGame();
	}
}

void GameScene::StartGame() {
	gameState_ = GameState::Playing;
	showStart_ = false;

	// 重置所有球体状态（如果需要）
	for (Ball* ball : balls_) {
		// 可以在这里重置球体位置和状态
	}
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
	for (Goal* goal : goals_) {
		goal->Draw();
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
	if (gameState_ != GameState::Playing) {
		DrawTutorial();
	}
	// 绘制所有球的爆炸范围（在前景）
	for (Ball* ball : balls_) {
		ball->DrawExplosionRange();
	}

	// 绘制游戏UI（总是在最上层）
	if (gameUI_) {
		gameUI_->Draw();
	}
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::DrawTutorial() {
    if (gameState_ == GameState::Tutorial && currentTutorialIndex_ < tutorialSprites_.size()) {
        // 绘制当前教程图片（全屏）
        tutorialSprites_[currentTutorialIndex_]->Draw();
    } else if ((gameState_ == GameState::StartWait || gameState_ == GameState::StartAnim) && 
               showStart_ && startSprite_) {
        // 在 StartWait 和 StartAnim 状态都绘制开始图片
        startSprite_->Draw();
    }
}



bool GameScene::IsMouseOverBall(Ball* ball, const Vector2& mousePos) {
	// 获取球体的世界坐标
	KamataEngine::Vector3 worldPos = ball->GetPosition();

	// 将世界坐标转换为屏幕坐标
	KamataEngine::Vector3 screenPos = WorldToScreen(worldPos);

	// 球体在屏幕上的半径（根据球体大小和距离调整）
	// 球体实际半径是2（从worldTransform_.scale_ = {2, 2, 2}推断）
	// 可视范围调整后的屏幕半径
	float ballRadius = 20.0f; // 这个值可能需要根据实际显示效果调整

	// 计算鼠标位置与球体屏幕位置的距离
	float distance = std::sqrt(std::pow(mousePos.x - screenPos.x, 2) + std::pow(mousePos.y - screenPos.y, 2));

	// 圆形碰撞检测：检查鼠标是否在圆形范围内
	return distance <= ballRadius;
}


KamataEngine::Vector3 GameScene::WorldToScreen(const KamataEngine::Vector3& worldPos) {
	// 确保相机矩阵已经更新
	camera_.UpdateMatrix();

	// 使用相机的视图投影矩阵进行准确的坐标转换
	const KamataEngine::Matrix4x4& viewMatrix = camera_.matView;
	const KamataEngine::Matrix4x4& projectionMatrix = camera_.matProjection;
	KamataEngine::Matrix4x4 viewProjectionMatrix = myMath::Multiply(viewMatrix, projectionMatrix);

	// 变换到齐次裁剪空间
	KamataEngine::Vector4 worldPos4 = {worldPos.x, worldPos.y, worldPos.z, 1.0f};

	// 手动进行矩阵变换（因为myMath::Transform可能不支持Vector4）
	KamataEngine::Vector4 clipPos;
	clipPos.x = worldPos4.x * viewProjectionMatrix.m[0][0] + worldPos4.y * viewProjectionMatrix.m[1][0] + worldPos4.z * viewProjectionMatrix.m[2][0] + worldPos4.w * viewProjectionMatrix.m[3][0];
	clipPos.y = worldPos4.x * viewProjectionMatrix.m[0][1] + worldPos4.y * viewProjectionMatrix.m[1][1] + worldPos4.z * viewProjectionMatrix.m[2][1] + worldPos4.w * viewProjectionMatrix.m[3][1];
	clipPos.z = worldPos4.x * viewProjectionMatrix.m[0][2] + worldPos4.y * viewProjectionMatrix.m[1][2] + worldPos4.z * viewProjectionMatrix.m[2][2] + worldPos4.w * viewProjectionMatrix.m[3][2];
	clipPos.w = worldPos4.x * viewProjectionMatrix.m[0][3] + worldPos4.y * viewProjectionMatrix.m[1][3] + worldPos4.z * viewProjectionMatrix.m[2][3] + worldPos4.w * viewProjectionMatrix.m[3][3];

	// 透视除法
	if (clipPos.w != 0.0f) {
		clipPos.x /= clipPos.w;
		clipPos.y /= clipPos.w;
		clipPos.z /= clipPos.w;
	}

	// 转换到屏幕坐标
	// 假设屏幕分辨率为1280x720
	float screenX = (clipPos.x + 1.0f) * 0.5f * 1280.0f;
	float screenY = (1.0f - (clipPos.y + 1.0f) * 0.5f) * 720.0f;

	return {screenX, screenY, clipPos.z};
}

bool GameScene::CheckBallGoalCollision(Ball* ball, Goal* goal) {
	// 获取球体和目标的位置
	KamataEngine::Vector3 ballPos = ball->GetPosition();
	KamataEngine::Vector3 goalPos = goal->GetPosition();

	// 计算距离
	float distance = myMath::Distance(ballPos, goalPos);

	// 碰撞半径（根据球体和目标的大小调整）
	float ballRadius = 2.0f;  // 球体半径（根据你的球体大小调整）
	float goalRadius = 2.0f;  // 目标半径（根据你的目标大小调整）
	float collisionRadius = ballRadius + goalRadius;

	// 调试输出
	// printf("Ball-Goal distance: %.2f, Collision radius: %.2f\n", distance, collisionRadius);

	return distance <= collisionRadius;
}

void GameScene::GameOver() {
	isGameOver_ = true;
	gameState_ = GameState::GameOver;
	
}

void GameScene::RestartLevel() {
	

	// 重置游戏状态
	isGameOver_ = false;
	gameState_ = GameState::Playing;

	// 重置所有球体
	for (Ball* ball : balls_) {
		// 重置球体到初始位置和状态
		ball->Reset();
	}

	// 重置UI点击状态
	if (gameUI_) {
		gameUI_->ResetClicks();
	}
}

void GameScene::ReturnToTitle() {
	
	isSceneEnd_ = true;
	returnToTitle_ = true;
	// 结束当前场景，返回主循环的标题状态
}