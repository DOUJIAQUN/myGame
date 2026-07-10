#include "GameScene.h"

#include "../DebugLogger.h"
#include "../play/ConcreteBallFactory.h"
#include "../play/GameOverState.h"
#include "../play/PlayingState.h"

#include <cassert>
#include <memory>
#include <utility>


using namespace KamataEngine;
namespace {
    const Vector2 kStartSpriteInitialSize = {3840.0f, 2160.0f};
    const Vector2 kStartSpriteTargetSize = {1280.0f, 720.0f};
    constexpr float kStartAnimDuration = 1.0f;
    constexpr float kStartDisplayDuration = 0.0f;
    constexpr float kScreenCenterX = 640.0f;
    constexpr float kScreenCenterY = 360.0f;
    constexpr float kHalf = 2.0f;
    constexpr float kFrameDeltaTime = 1.0f / 60.0f;
    constexpr float kStartWaitDuration = 0.01f;
    constexpr int kLeftMouseButton = 0;
}


namespace MyEngine {

GameScene::GameScene() {
    ballFactory_ = std::make_unique<ConcreteBallFactory>();
}

GameScene::~GameScene() = default;

void GameScene::Initialize() {
    dxCommon_ = DirectXCommon::GetInstance();
    input_ = Input::GetInstance();

    camera_.Initialize();

    stage_ = std::make_unique<Stage>();
    stage_->Initialize();

    gameUI_ = std::make_unique<GameUI>();
    gameUI_->Initialize();

    startSize_ = kStartSpriteInitialSize;
    targetSize_ = kStartSpriteTargetSize;
    animDuration_ = kStartAnimDuration;
    displayDuration_ = kStartDisplayDuration;

    LoadTutorialTextures();

    isSceneEnd_ = false;
    returnToTitle_ = false;

    if (levelNumber_ == 1) {
        gameFlowState_ = GameFlowState::Tutorial;
        currentTutorialIndex_ = 0;
        showStart_ = false;
        currentState_.reset();
    }
    else {
        // 第一关以外はチュートリアルをスキップして直接 Playing 状態へ
        gameFlowState_ = GameFlowState::Tutorial;
        currentTutorialIndex_ = tutorialSprites_.size();
        showStart_ = false;

        currentState_ = std::make_unique<PlayingState>();
        currentState_->Enter(this);
    }

    InitializeLevelObjects();
    gameLogicManager_.Initialize(balls_, goals_, &camera_);
}

void GameScene::Update() {
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

    if (currentState_) {
        currentState_->Update(this);
    }

    camera_.UpdateMatrix();
}

void GameScene::Draw() {
    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

    Sprite::PreDraw(commandList);
    if (stage_) {
        stage_->Draw();
    }
    Sprite::PostDraw();

    dxCommon_->ClearDepthBuffer();

    Model::PreDraw(commandList);
    for (const auto& ball : balls_) {
        if (ball) {
            ball->Draw();
        }
    }

    for (const auto& goal : goals_) {
        if (goal) {
            goal->Draw();
        }
    }
    Model::PostDraw();

    Sprite::PreDraw(commandList);

    DrawTutorial();

    for (const auto& ball : balls_) {
        if (ball) {
            ball->DrawExplosionRange();
        }
    }

    if (gameUI_) {
        gameUI_->Draw();
    }

    Sprite::PostDraw();
}

void GameScene::ChangeState(std::unique_ptr<IGameState> newState) {
    if (currentState_) {
        currentState_->Exit(this);
    }

    currentState_ = std::move(newState);

    if (currentState_) {
        currentState_->Enter(this);
    }
}

void GameScene::UpdateStageInternal() {
    if (stage_) {
        stage_->Update();
    }
}

void GameScene::UpdateUIInternal() {
    if (!gameUI_) {
        return;
    }

    gameUI_->Update();

    if (gameUI_->IsRestartClicked()) {
        RestartLevel();
    }
    else if (gameUI_->IsReturnToTitleClicked()) {
        ReturnToTitle();
    }
}

void GameScene::UpdateGameLogicInternal() {
    gameLogicManager_.Update();
}

void GameScene::UpdateBallsAndGoalsInternal() {
    for (const auto& ball : balls_) {
        if (ball) {
            ball->Update();
        }
    }

    for (const auto& goal : goals_) {
        if (goal) {
            goal->Update();
        }
    }
}

bool GameScene::IsLevelComplete() const {
    for (const auto& goal : goals_) {
        if (goal && !goal->IsCompleted()) {
            return false;
        }
    }

    return true;
}

void GameScene::SetSceneEndFlag() {
    isSceneEnd_ = true;
}

void GameScene::RestartLevel() {
    gameFlowState_ = GameFlowState::StartWait;

    if (currentState_) {
        currentState_->Exit(this);
        currentState_.reset();
    }

    gameLogicManager_.Reset();
    ResetLevelCompletion();

    if (gameUI_) {
        gameUI_->ResetClicks();
    }

    InitializeLevelObjects();
    gameLogicManager_.Initialize(balls_, goals_, &camera_);
}

void GameScene::ReturnToTitle() {
    isSceneEnd_ = true;
    returnToTitle_ = true;
}

void GameScene::GameOver() {
    if (dynamic_cast<GameOverState*>(currentState_.get()) == nullptr) {
        ChangeState(std::make_unique<GameOverState>());
    }
}

void GameScene::StartGame() {
    showStart_ = false;
    gameFlowState_ = GameFlowState::Tutorial;

    ChangeState(std::make_unique<PlayingState>());
}

void GameScene::LoadTutorialTextures() {
    tutorialTextureHandles_.clear();
    tutorialSprites_.clear();

    tutorialTextureHandles_.push_back(TextureManager::Load("gameTutorial/tutorial1.png"));
    tutorialTextureHandles_.push_back(TextureManager::Load("gameTutorial/tutorial2.png"));
    tutorialTextureHandles_.push_back(TextureManager::Load("gameTutorial/tutorial3.png"));
    tutorialTextureHandles_.push_back(TextureManager::Load("gameTutorial/tutorial4.png"));

    for (uint32_t handle : tutorialTextureHandles_) {
        std::unique_ptr<KamataEngine::Sprite> sprite;
        sprite.reset(Sprite::Create(handle, { 0, 0 }));
        tutorialSprites_.push_back(std::move(sprite));
    }

    startTextureHandle_ = TextureManager::Load("gameTutorial/gameStart.png");

    float startX = kScreenCenterX - (startSize_.x / kHalf);
    float startY = kScreenCenterY - (startSize_.y / kHalf);

    startSprite_.reset(Sprite::Create(startTextureHandle_, { startX, startY }));

    if (startSprite_) {
        startSprite_->SetSize(startSize_);
    }
}

void GameScene::UpdateTutorial() {
    if (currentTutorialIndex_ >= tutorialSprites_.size()) {
        return;
    }

    if (input_->IsTriggerMouse(kLeftMouseButton)) {
        currentTutorialIndex_++;

        if (currentTutorialIndex_ >= tutorialSprites_.size()) {
            showStart_ = true;
            gameFlowState_ = GameFlowState::StartWait;
            startTimer_ = 0.0f;
        }
    }
}

void GameScene::UpdateStartWait() {
    startTimer_ += kFrameDeltaTime;

    if (input_->IsTriggerMouse(kLeftMouseButton) || startTimer_ >= kStartWaitDuration) {
        gameFlowState_ = GameFlowState::StartAnim;
        animTimer_ = 0.0f;

        if (startSprite_) {
            float startX = kScreenCenterX - (startSize_.x / kHalf);
            float startY = kScreenCenterY - (startSize_.y / kHalf);

            startSprite_->SetPosition({ startX, startY });
            startSprite_->SetSize(startSize_);
        }
    }
}

void GameScene::UpdateStartAnim() {
    animTimer_ += kFrameDeltaTime;

    if (!startSprite_) {
        StartGame();
        return;
    }

    float progress = animTimer_ / animDuration_;
    if (progress > 1.0f) {
        progress = 1.0f;
    }

    float easeProgress = 1.0f - (1.0f - progress) * (1.0f - progress) * (1.0f - progress);

    float currentSizeX = startSize_.x + (targetSize_.x - startSize_.x) * easeProgress;
    float currentSizeY = startSize_.y + (targetSize_.y - startSize_.y) * easeProgress;

    float currentX = kScreenCenterX - (currentSizeX / kHalf);
    float currentY = kScreenCenterY - (currentSizeY / kHalf);

    startSprite_->SetSize({ currentSizeX, currentSizeY });
    startSprite_->SetPosition({ currentX, currentY });

    if (progress >= 1.0f && animTimer_ >= animDuration_ + displayDuration_) {
        StartGame();
    }
}

void GameScene::DrawTutorial() {
    if (gameFlowState_ == GameFlowState::Tutorial &&
        currentTutorialIndex_ < tutorialSprites_.size() &&
        tutorialSprites_[currentTutorialIndex_]) {

        tutorialSprites_[currentTutorialIndex_]->Draw();
    }
    else if ((gameFlowState_ == GameFlowState::StartWait ||
        gameFlowState_ == GameFlowState::StartAnim) &&
        showStart_ &&
        startSprite_) {

        startSprite_->Draw();
    }
}

void GameScene::SetLevelConfig(
    int levelNumber,
    const std::vector<Vector3>& ballPositions,
    const std::vector<Vector3>& goalPositions,
    const std::vector<int>& goalRequiredCounts,
    const std::vector<GoalMovementConfig>& goalMovementConfigs
) {
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
    balls_.clear();
    goals_.clear();

    for (const auto& pos : levelBallPositions_) {
        std::unique_ptr<MyEngine::Ball> ball = ballFactory_->CreateBall(pos, levelNumber_);

        ball->Initialize(&camera_);
        ball->SetInitialPosition(pos);

        balls_.push_back(std::move(ball));
    }

    for (size_t i = 0; i < levelGoalPositions_.size(); i++) {
        auto goal = std::make_unique<MyEngine::Goal>();

        goal->Initialize(&camera_);
        goal->SetPosition(levelGoalPositions_[i]);

        if (i < levelGoalRequiredCounts_.size()) {
            goal->SetRequiredCount(levelGoalRequiredCounts_[i]);
        }

        if (i < levelGoalMovementConfigs_.size()) {
            goal->SetMovementConfig(levelGoalMovementConfigs_[i]);
        }

        goals_.push_back(std::move(goal));
    }

    ResetLevelCompletion();
}

void GameScene::ResetLevelCompletion() {
    for (const auto& goal : goals_) {
        if (goal) {
            goal->ResetCount();
        }
    }
}

void GameScene::CheckLevelCompletion() {
    if (IsLevelComplete()) {
        GameOver();
    }
}

void GameScene::SetBallFactory(std::unique_ptr<IBallFactory> factory) {
    ballFactory_ = std::move(factory);
}

} // namespace MyEngine
