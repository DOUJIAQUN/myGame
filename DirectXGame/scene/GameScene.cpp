#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete stage_;
	for (Ball* ball : balls_) {
		delete ball;
	}
	balls_.clear(); // 清空向量

	for (Goal* goal : goals_) {
		delete goal;
	}
	goals_.clear();

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


	// 根据关卡号决定初始状态
	if (levelNumber_ == 1) {
		// 第一关：正常教程流程
		gameState_ = GameState::Tutorial;
		currentTutorialIndex_ = 0;
		showStart_ = false;
	}
	else {
		// 第二关及以后：直接显示开始图片，跳过教程
		gameState_ = GameState::Playing;
		showStart_ = true;
		startTimer_ = 0.0f;
	}



	// 初始化游戏对象
	InitializeLevelObjects();

	// 初始化游戏逻辑管理器
	gameLogicManager_.Initialize(balls_, goals_, &camera_);
}

void GameScene::LoadTutorialTextures() {
	// 加载教程图片纹理并创建对应的Sprite
	tutorialTextureHandles_.push_back(TextureManager::Load("gameTutorial/tutorial1.png"));
	tutorialTextureHandles_.push_back(TextureManager::Load("gameTutorial/tutorial2.png"));
	tutorialTextureHandles_.push_back(TextureManager::Load("gameTutorial/tutorial3.png"));
	tutorialTextureHandles_.push_back(TextureManager::Load("gameTutorial/tutorial4.png"));

	// 为每个纹理创建Sprite对象（全屏显示）
	for (auto handle : tutorialTextureHandles_) {
		KamataEngine::Sprite* sprite = Sprite::Create(handle, { 0, 0 });
		tutorialSprites_.push_back(sprite);
	}

	// 加载开始图片
	startTextureHandle_ = TextureManager::Load("gameTutorial/gameStart.png");
	// 计算初始位置：让3倍大的图片居中显示
	// 屏幕中心坐标 - (图片宽度/2)
	float startX = 640.0f - (startSize_.x / 2.0f);
	float startY = 360.0f - (startSize_.y / 2.0f);
	startSprite_ = Sprite::Create(startTextureHandle_, { startX, startY });

	// 设置初始尺寸（3倍大小）
	if (startSprite_) {
		startSprite_->SetSize(startSize_);
	}
}


// 新增 InitializeLevelObjects 方法：
void GameScene::InitializeLevelObjects() {
	// 清理现有的游戏对象
	for (Ball* ball : balls_) {
		delete ball;
	}
	balls_.clear();

	for (Goal* goal : goals_) {
		delete goal;
	}
	goals_.clear();

	// 创建球体
	for (const auto& position : levelBallPositions_) {
		Ball* ball = new Ball();
		ball->Initialize(&camera_);
		ball->SetInitialPosition(position);
		balls_.push_back(ball);
	}

	// 创建多个目标并设置需求次数
	for (size_t i = 0; i < levelGoalPositions_.size(); i++) {
		Goal* goal = new Goal();
		goal->Initialize(&camera_);
		goal->SetPosition(levelGoalPositions_[i]);

		// 设置每个终点的需求次数
		if (i < levelGoalRequiredCounts_.size()) {
			goal->SetRequiredCount(levelGoalRequiredCounts_[i]);
		}

		goals_.push_back(goal);
	}
	// 重置通关状态
	ResetLevelCompletion();
}



// 新增 SetLevelConfig 方法：
void GameScene::SetLevelConfig(int levelNumber,
	const std::vector<KamataEngine::Vector3>& ballPositions,
	const std::vector<KamataEngine::Vector3>& goalPositions,
	const std::vector<int>& goalRequiredCounts) {
	levelNumber_ = levelNumber;
	levelBallPositions_ = ballPositions;
	levelGoalPositions_ = goalPositions;
	levelGoalRequiredCounts_ = goalRequiredCounts;



	// 如果没提供需求次数，默认每个终点需要1次
	if (levelGoalRequiredCounts_.empty()) {
		levelGoalRequiredCounts_.resize(goalPositions.size(), 1);
	}
}


// 新增检查关卡完成的方法
void GameScene::CheckLevelCompletion() {
	// 检查每个终点是否满足需求次数
	bool allGoalsCompleted = true;

	for (Goal* goal : goals_) {
		if (!goal->IsCompleted()) {
			allGoalsCompleted = false;
			break;
		}
	}

	// 如果所有终点都满足需求次数，则关卡完成
	if (allGoalsCompleted) {
		GameOver();
	}
}

// 新增重置完成状态的方法
void GameScene::ResetLevelCompletion() {
	// 重置所有终点的计数
	for (Goal* goal : goals_) {
		goal->ResetCount();
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


		// 使用游戏逻辑管理器更新游戏逻辑
		gameLogicManager_.Update();


		
		// 更新所有 Ball
		for (Ball* ball : balls_) {
			ball->Update();
		}
		
		// 检查关卡完成条件
		CheckLevelCompletion();

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


void GameScene::GameOver() {
	
	gameState_ = GameState::GameOver;
	
}

void GameScene::RestartLevel() {
	

	// 重置游戏状态
	
	gameState_ = GameState::Playing;
	gameLogicManager_.Reset();
	ResetLevelCompletion();

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


