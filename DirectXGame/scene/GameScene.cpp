#include "GameScene.h"

#include "../DebugLogger.h"
#include "../play/ConcreteBallFactory.h"
#include "../play/GameOverState.h"
#include "../play/PlayingState.h"

#include <cassert>
#include <memory>
#include <utility>

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

// 関数コメント: GameScene の処理を実行する。
GameScene::GameScene() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、GameScene の役割を実現する。
    ballFactory_ = std::make_unique<ConcreteBallFactory>();
}

GameScene::~GameScene() = default;

// 関数コメント: Initialize の処理を実行する。
void GameScene::Initialize() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Initialize の役割を実現する。
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

// 関数コメント: Update の処理を実行する。
void GameScene::Update() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Update の役割を実現する。
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

// 関数コメント: Draw の処理を実行する。
void GameScene::Draw() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Draw の役割を実現する。
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

// 関数コメント: ChangeState の処理を実行する。
void GameScene::ChangeState(std::unique_ptr<IGameState> newState) {
    // 処理コメント: 必要な状態確認やデータ更新を行い、ChangeState の役割を実現する。
    if (currentState_) {
        currentState_->Exit(this);
    }

    currentState_ = std::move(newState);

    if (currentState_) {
        currentState_->Enter(this);
    }
}

// 関数コメント: UpdateStageInternal の処理を実行する。
void GameScene::UpdateStageInternal() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、UpdateStageInternal の役割を実現する。
    if (stage_) {
        stage_->Update();
    }
}

// 関数コメント: UpdateUIInternal の処理を実行する。
void GameScene::UpdateUIInternal() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、UpdateUIInternal の役割を実現する。
    if (!gameUI_) {
        return;
    }

    gameUI_->Update();

    if (gameUI_->IsRestartClicked()) {
        RestartLevel();
    }
    // 関数コメント: if の処理を実行する。
    else if (gameUI_->IsReturnToTitleClicked()) {
        // 処理コメント: 必要な状態確認やデータ更新を行い、if の役割を実現する。
        ReturnToTitle();
    }
}

// 関数コメント: UpdateGameLogicInternal の処理を実行する。
void GameScene::UpdateGameLogicInternal() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、UpdateGameLogicInternal の役割を実現する。
    gameLogicManager_.Update();
}

// 関数コメント: UpdateBallsAndGoalsInternal の処理を実行する。
void GameScene::UpdateBallsAndGoalsInternal() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、UpdateBallsAndGoalsInternal の役割を実現する。
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

// 関数コメント: IsLevelComplete の処理を実行する。
bool GameScene::IsLevelComplete() const {
    // 処理コメント: 必要な状態確認やデータ更新を行い、IsLevelComplete の役割を実現する。
    for (const auto& goal : goals_) {
        if (goal && !goal->IsCompleted()) {
            return false;
        }
    }

    return true;
}

// 関数コメント: SetSceneEndFlag の処理を実行する。
void GameScene::SetSceneEndFlag() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、SetSceneEndFlag の役割を実現する。
    isSceneEnd_ = true;
}

// 関数コメント: RestartLevel の処理を実行する。
void GameScene::RestartLevel() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、RestartLevel の役割を実現する。
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

// 関数コメント: ReturnToTitle の処理を実行する。
void GameScene::ReturnToTitle() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、ReturnToTitle の役割を実現する。
    isSceneEnd_ = true;
    returnToTitle_ = true;
}

// 関数コメント: GameOver の処理を実行する。
void GameScene::GameOver() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、GameOver の役割を実現する。
    if (dynamic_cast<GameOverState*>(currentState_.get()) == nullptr) {
        ChangeState(std::make_unique<GameOverState>());
    }
}

// 関数コメント: StartGame の処理を実行する。
void GameScene::StartGame() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、StartGame の役割を実現する。
    showStart_ = false;
    gameFlowState_ = GameFlowState::Tutorial;

    ChangeState(std::make_unique<PlayingState>());
}

// 関数コメント: LoadTutorialTextures の処理を実行する。
void GameScene::LoadTutorialTextures() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、LoadTutorialTextures の役割を実現する。
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

// 関数コメント: UpdateTutorial の処理を実行する。
void GameScene::UpdateTutorial() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、UpdateTutorial の役割を実現する。
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

// 関数コメント: UpdateStartWait の処理を実行する。
void GameScene::UpdateStartWait() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、UpdateStartWait の役割を実現する。
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

// 関数コメント: UpdateStartAnim の処理を実行する。
void GameScene::UpdateStartAnim() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、UpdateStartAnim の役割を実現する。
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

// 関数コメント: DrawTutorial の処理を実行する。
void GameScene::DrawTutorial() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、DrawTutorial の役割を実現する。
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

// 関数コメント: InitializeLevelObjects の処理を実行する。
void GameScene::InitializeLevelObjects() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、InitializeLevelObjects の役割を実現する。
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

// 関数コメント: ResetLevelCompletion の処理を実行する。
void GameScene::ResetLevelCompletion() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、ResetLevelCompletion の役割を実現する。
    for (const auto& goal : goals_) {
        if (goal) {
            goal->ResetCount();
        }
    }
}

// 関数コメント: CheckLevelCompletion の処理を実行する。
void GameScene::CheckLevelCompletion() {
    // 処理コメント: 必要な状態確認やデータ更新を行い、CheckLevelCompletion の役割を実現する。
    if (IsLevelComplete()) {
        GameOver();
    }
}

// 関数コメント: SetBallFactory の処理を実行する。
void GameScene::SetBallFactory(std::unique_ptr<IBallFactory> factory) {
    // 処理コメント: 必要な状態確認やデータ更新を行い、SetBallFactory の役割を実現する。
    ballFactory_ = std::move(factory);
}
