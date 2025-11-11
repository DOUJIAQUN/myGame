#include "LevelManager.h"
#include <cassert>

LevelManager::LevelManager()
    : currentLevelIndex_(0),
    shouldReturnToTitle_(false),
    isSceneEnd_(false),
    currentState_(LevelState::Playing) {
    CreateLevels();
    loadingScene_ = new LoadingScene();
    loadingScene_->Initialize();
}

LevelManager::~LevelManager() {
    CleanupLevels();
    if (loadingScene_) {
        delete loadingScene_;
    }
}

void LevelManager::Initialize() {
    if (!levels_.empty()) {
        levels_[0]->Initialize();
        isSceneEnd_ = false;
        shouldReturnToTitle_ = false;
        currentState_ = LevelState::Playing;
    }
}

void LevelManager::Update() {
    if (isSceneEnd_) return;

    switch (currentState_) {
    case LevelState::Playing:
        UpdatePlayingState();
        break;

    case LevelState::Loading:
        UpdateLoadingState();
        break;

    case LevelState::Transition:
        UpdateTransitionState();
        break;
    }
}

void LevelManager::UpdatePlayingState() {
    if (currentLevelIndex_ >= levels_.size()) return;

    GameScene* currentLevel = levels_[currentLevelIndex_];
    if (!currentLevel) return;

    currentLevel->Update();

    // 检查当前关卡是否结束
    if (currentLevel->IsSceneEnd()) {
        SceneState nextState = currentLevel->GetNextSceneState();

        if (nextState == TITLE) {
            // 返回标题
            shouldReturnToTitle_ = true;
            isSceneEnd_ = true;
        }
        else {
            // 正常通关，开始加载下一关
            StartLevelTransition();
        }
    }
}

void LevelManager::UpdateLoadingState() {
    loadingScene_->Updata();
    if (loadingScene_->isLoadingComplete()) {
        // 加载完成，进入下一关
        currentLevelIndex_++;
        if (currentLevelIndex_ < levels_.size()) {
            levels_[currentLevelIndex_]->Initialize();
            currentState_ = LevelState::Playing;
            printf("进入第 %zu 关\n", currentLevelIndex_ + 1);
        }
        else {
            // 所有关卡完成
            printf("所有关卡完成！\n");
            isSceneEnd_ = true;
        }
    }
}

void LevelManager::UpdateTransitionState() {
    // 可以在这里添加其他过渡效果
    // 暂时为空
}

void LevelManager::Draw() {
    switch (currentState_) {
    case LevelState::Playing:
        if (currentLevelIndex_ < levels_.size() && levels_[currentLevelIndex_]) {
            levels_[currentLevelIndex_]->Draw();
        }
        break;

    case LevelState::Loading:
        if (loadingScene_) {
            loadingScene_->Draw();
        }
        break;

    case LevelState::Transition:
        // 绘制过渡效果
        break;
    }
}

void LevelManager::StartLevelTransition() {
    if (currentLevelIndex_ + 1 < levels_.size()) {
        currentState_ = LevelState::Loading;
        loadingScene_->StartLoading();
       
    }
    else {
        // 已经是最后一关
        isSceneEnd_ = true;
    }
}

void LevelManager::GoToNextLevel() {
    currentLevelIndex_++;

    if (currentLevelIndex_ < levels_.size()) {
        // 初始化下一关
       
        levels_[currentLevelIndex_]->Initialize();
    }
    else {
        // 所有关卡完成
       
        isSceneEnd_ = true;
    }
}

void LevelManager::RestartCurrentLevel() {
    if (currentLevelIndex_ < levels_.size() && levels_[currentLevelIndex_]) {
        levels_[currentLevelIndex_]->Initialize();
    }
}

void LevelManager::ReturnToTitle() {
    shouldReturnToTitle_ = true;
    isSceneEnd_ = true;
}

void LevelManager::CreateLevels() {
    // 第一关配置
    GameScene* level1 = new GameScene();
    std::vector<KamataEngine::Vector3> level1BallPositions = {
        {-30.0f, 0.0f, 0.0f},
        {-20.0f, 0.0f, 0.0f},
        { 0.0f, 0.0f, 0.0f},
        {15.0f, 0.0f, 0.0f}
    };
    std::vector<KamataEngine::Vector3> level1GoalPositions = {
       {25.0f, 0.0f, 0.0f}  // 单个终点
    };
    level1->SetLevelConfig(1, level1BallPositions, level1GoalPositions);
    levels_.push_back(level1);

    // 第二关配置
    GameScene* level2 = new GameScene();
    std::vector<KamataEngine::Vector3> level2BallPositions = {
        {-12.0f, 0.0f, 0.0f},
        {-7.0f, -5.0f, 0.0f},
        {-7.0f, 5.0f, 0.0f},
        {7.0f, -5.0f, 0.0f},
        {7.0f, 5.0f, 0.0f},
        { 12.0f, 0.0f, 0.0f }
    };
    std::vector<KamataEngine::Vector3> level2GoalPositions = {
        { 0.0f, 12.0f, 0.0f },
        { 0.0f, -12.0f, 0.0f }
    };
    std::vector<int> level2GoalRequiredCounts = {
    2,  // 终点1需要进入2次
    2   // 终点2需要进入2次
    };
    level2->SetLevelConfig(2, level2BallPositions, level2GoalPositions,level2GoalRequiredCounts);
    levels_.push_back(level2);
   
}

void LevelManager::CleanupLevels() {
    for (auto level : levels_) {
        if (level) {
            delete level;
        }
    }
    levels_.clear();
}