#include "GameScene.h"
#include "../play/PlayingState.h"
#include "../play/GameOverState.h"
#include <cassert>
#include "../DebugLogger.h"
#include "../play/ConcreteBallFactory.h"

GameScene::GameScene() { ballFactory_ = new ConcreteBallFactory(); }

GameScene::~GameScene() {
    delete stage_;
    for (Ball* ball : balls_) delete ball;
    balls_.clear();
    for (Goal* goal : goals_) delete goal;
    goals_.clear();
    for (auto sprite : tutorialSprites_) delete sprite;
    tutorialSprites_.clear();
    delete startSprite_;
    delete gameUI_;
    if (currentState_) delete currentState_;
    delete ballFactory_;
}

void GameScene::Initialize() {
    dxCommon_ = DirectXCommon::GetInstance();
    input_ = Input::GetInstance();
    camera_.Initialize();

    stage_ = new Stage();
    stage_->Initialize();

    gameUI_ = new GameUI();
    gameUI_->Initialize();

    LoadTutorialTextures();

    // 根据关卡号决定初始流程状态
    if (levelNumber_ == 1) {
        gameFlowState_ = GameFlowState::Tutorial;
        currentTutorialIndex_ = 0;
        showStart_ = false;
        currentState_ = nullptr;
    }
    else {
        gameFlowState_ = GameFlowState::Tutorial;       // 随便设一个，但保证不会绘制教程
        currentTutorialIndex_ = (int)tutorialSprites_.size(); // 索引越界，不绘制教程
        showStart_ = false;
        // 直接进入 Playing 状态
        currentState_ = new PlayingState();
        currentState_->Enter(this);
    }

    InitializeLevelObjects();
    gameLogicManager_.Initialize(balls_, goals_, &camera_);

  
}

void GameScene::Update() {
    // 处理前三个流程状态（未使用状态模式）
    switch (gameFlowState_) {
    case GameFlowState::Tutorial:
        UpdateTutorial();
        break;
    case GameFlowState::StartWait:
        UpdateStartWait();
        break;
    case GameFlowState::StartAnim:
        UpdateStartAnim();
        break;
    }

    // 如果处于 Playing/GameOver 状态，则通过状态模式更新
    if (currentState_) {
        currentState_->Update(this);
    }

    camera_.UpdateMatrix();
}

// ---- 状态模式公共接口实现 ----
void GameScene::ChangeState(IGameState* newState) {
    if (currentState_) {
        currentState_->Exit(this);
        delete currentState_;
    }
    currentState_ = newState;
    currentState_->Enter(this);
}

void GameScene::UpdateStageInternal() {
    stage_->Update();
}

void GameScene::UpdateUIInternal() {
    if (gameUI_) {
        gameUI_->Update();
        if (gameUI_->IsRestartClicked()) {
            RestartLevel();
        }
        else if (gameUI_->IsReturnToTitleClicked()) {
            ReturnToTitle();
        }
    }
}

void GameScene::UpdateGameLogicInternal() {
    gameLogicManager_.Update();
}

void GameScene::UpdateBallsAndGoalsInternal() {
    for (Ball* ball : balls_) {
        ball->Update();
    }
    for (Goal* goal : goals_) {
        goal->Update();
    }
}

bool GameScene::IsLevelComplete() const {
    for (Goal* goal : goals_) {
        if (!goal->IsCompleted()) return false;
    }
    return true;
}

void GameScene::SetSceneEndFlag() {
    isSceneEnd_ = true;
}

void GameScene::RestartLevel() {
    // 重置游戏状态
    gameFlowState_ = GameFlowState::StartWait; // 根据需求可改为直接 Playing
    if (currentState_) {
        delete currentState_;
        currentState_ = nullptr;
    }
    gameLogicManager_.Reset();
    ResetLevelCompletion();

    for (Ball* ball : balls_) {
        ball->Reset();
    }
    if (gameUI_) {
        gameUI_->ResetClicks();
    }
    // 重新初始化关卡物体（位置重置）
    InitializeLevelObjects();
    // 如果是第一关，可能需要回到教程等逻辑，可自行调整
}

void GameScene::ReturnToTitle() {
    isSceneEnd_ = true;
    returnToTitle_ = true;
}

void GameScene::GameOver() {
    // 如果当前不是 GameOver 状态，则切换
    if (dynamic_cast<GameOverState*>(currentState_) == nullptr) {
        ChangeState(new GameOverState());
    }
}

// ---- StartGame：由动画结束时调用 ----
void GameScene::StartGame() {
    showStart_ = false;                         // 隐藏 start 图片
    gameFlowState_ = GameFlowState::Tutorial;   // 离开 StartAnim 状态，防止再次绘制
    // 进入 Playing 状态
    ChangeState(new PlayingState());
}

// ---- 以下为原有的各种内部函数，内容保持原样（仅微调） ----
void GameScene::LoadTutorialTextures() {
    tutorialTextureHandles_.push_back(TextureManager::Load("gameTutorial/tutorial1.png"));
    tutorialTextureHandles_.push_back(TextureManager::Load("gameTutorial/tutorial2.png"));
    tutorialTextureHandles_.push_back(TextureManager::Load("gameTutorial/tutorial3.png"));
    tutorialTextureHandles_.push_back(TextureManager::Load("gameTutorial/tutorial4.png"));

    for (auto handle : tutorialTextureHandles_) {
        KamataEngine::Sprite* sprite = Sprite::Create(handle, { 0, 0 });
        tutorialSprites_.push_back(sprite);
    }

    startTextureHandle_ = TextureManager::Load("gameTutorial/gameStart.png");
    float startX = 640.0f - (startSize_.x / 2.0f);
    float startY = 360.0f - (startSize_.y / 2.0f);
    startSprite_ = Sprite::Create(startTextureHandle_, { startX, startY });
    if (startSprite_) {
        startSprite_->SetSize(startSize_);
    }
}

void GameScene::UpdateTutorial() {
    if (currentTutorialIndex_ >= (int)tutorialSprites_.size()) {
        return;
    }
    if (input_->IsTriggerMouse(0)) {
        currentTutorialIndex_++;
        if (currentTutorialIndex_ >= tutorialSprites_.size()) {
            showStart_ = true;
            gameFlowState_ = GameFlowState::StartWait;
            startTimer_ = 0.0f;
        }
    }
}

void GameScene::UpdateStartWait() {
    startTimer_ += 1.0f / 60.0f;
    if (input_->IsTriggerMouse(0) || startTimer_ >= 0.01f) {
        gameFlowState_ = GameFlowState::StartAnim;
        animTimer_ = 0.0f;
        if (startSprite_) {
            float startX = 640.0f - (startSize_.x / 2.0f);
            float startY = 360.0f - (startSize_.y / 2.0f);
            startSprite_->SetPosition({ startX, startY });
            startSprite_->SetSize(startSize_);
        }
    }
}

void GameScene::UpdateStartAnim() {
    animTimer_ += 1.0f / 60.0f;
    if (startSprite_) {
        float progress = animTimer_ / animDuration_;
        if (progress > 1.0f) progress = 1.0f;
        float easeProgress = 1.0f - (1.0f - progress) * (1.0f - progress) * (1.0f - progress);
        float currentSizeX = startSize_.x + (targetSize_.x - startSize_.x) * easeProgress;
        float currentSizeY = startSize_.y + (targetSize_.y - startSize_.y) * easeProgress;
        float currentX = 640.0f - (currentSizeX / 2.0f);
        float currentY = 360.0f - (currentSizeY / 2.0f);
        startSprite_->SetSize({ currentSizeX, currentSizeY });
        startSprite_->SetPosition({ currentX, currentY });

        if (progress >= 1.0f && animTimer_ >= animDuration_ + displayDuration_) {
            StartGame();
        }
    }
    else {
        StartGame();
    }
}

void GameScene::DrawTutorial() {
    if (gameFlowState_ == GameFlowState::Tutorial && currentTutorialIndex_ < tutorialSprites_.size()) {
        tutorialSprites_[currentTutorialIndex_]->Draw();
    }
    else if ((gameFlowState_ == GameFlowState::StartWait || gameFlowState_ == GameFlowState::StartAnim) && showStart_ && startSprite_) {
        startSprite_->Draw();
    }
}

void GameScene::Draw() {
    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

    Sprite::PreDraw(commandList);
    stage_->Draw();
    Sprite::PostDraw();
    dxCommon_->ClearDepthBuffer();

    Model::PreDraw(commandList);
    for (Ball* ball : balls_) ball->Draw();
    for (Goal* goal : goals_) goal->Draw();
    Model::PostDraw();

    Sprite::PreDraw(commandList);
    DrawTutorial();
    for (Ball* ball : balls_) ball->DrawExplosionRange();
    if (gameUI_) gameUI_->Draw();
    Sprite::PostDraw();
}

// ---- 以下为关卡配置相关函数（内容保持原样） ----
void GameScene::SetLevelConfig(int levelNumber,
    const std::vector<Vector3>& ballPositions,
    const std::vector<Vector3>& goalPositions,
    const std::vector<int>& goalRequiredCounts,
    const std::vector<GoalMovementConfig>& goalMovementConfigs) {
    levelNumber_ = levelNumber;
    levelBallPositions_ = ballPositions;
    levelGoalPositions_ = goalPositions;
    levelGoalRequiredCounts_ = goalRequiredCounts;
    levelGoalMovementConfigs_ = goalMovementConfigs;

    if (levelGoalRequiredCounts_.empty()) {
        levelGoalRequiredCounts_.resize(goalPositions.size(), 1);
    }
    if (levelGoalMovementConfigs_.empty()) {
        levelGoalMovementConfigs_.resize(goalPositions.size(), GoalMovementConfig());
    }
}

void GameScene::InitializeLevelObjects() {
    for (Ball* ball : balls_) delete ball;
    balls_.clear();
    for (Goal* goal : goals_) delete goal;
    goals_.clear();

    // 使用工厂创建球
    for (const auto& pos : levelBallPositions_) {
        Ball* ball = ballFactory_->CreateBall(pos, levelNumber_);
        ball->Initialize(&camera_);
        ball->SetInitialPosition(pos);
        balls_.push_back(ball);
    }

    for (size_t i = 0; i < levelGoalPositions_.size(); i++) {
        Goal* goal = new Goal();
        goal->Initialize(&camera_);
        goal->SetPosition(levelGoalPositions_[i]);
        if (i < levelGoalRequiredCounts_.size())
            goal->SetRequiredCount(levelGoalRequiredCounts_[i]);
        if (i < levelGoalMovementConfigs_.size())
            goal->SetMovementConfig(levelGoalMovementConfigs_[i]);
        goals_.push_back(goal);
    }
    ResetLevelCompletion();
}

void GameScene::ResetLevelCompletion() {
    for (Goal* goal : goals_) {
        goal->ResetCount();
    }
}

void GameScene::CheckLevelCompletion() {
    if (IsLevelComplete()) {
        GameOver();
    }
}

void GameScene::SetBallFactory(IBallFactory* factory) {
    if (ballFactory_) delete ballFactory_;
    ballFactory_ = factory;
}