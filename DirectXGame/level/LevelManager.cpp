#include "LevelManager.h"

#include <cassert>
#include "../DebugLogger.h"

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
        loadingScene_ = nullptr;
    }
}

std::string LevelManager::GetCurrentLevelName() const {
    if (currentLevelIndex_ >= levelNames_.size()) {
        return "Unknown";
    }

    return levelNames_[currentLevelIndex_];
}

std::vector<LevelManager::LevelConfig> LevelManager::CreateLevelConfigs() const {
    std::vector<LevelConfig> configs = {
        {
            1,
            "1-1",

            // Ball positions
            {
                {-30.0f, 0.0f, 0.0f},
                {-20.0f, 0.0f, 0.0f},
                {  0.0f, 0.0f, 0.0f},
                { 20.0f, 0.0f, 0.0f}
            },

        // Goal positions
        {
            {30.0f, 0.0f, 0.0f}
        },

        // Goal required counts
        {
            1
        },

        // Goal movement configs
        {
            GoalMovementConfig(false, MoveDirection::Horizontal, 0.0f, 0.0f)
        }
    },

    {
        2,
        "1-2",

        // Ball positions
        {
            {-12.0f,  0.0f, 0.0f},
            { -7.0f, -5.0f, 0.0f},
            { -7.0f,  5.0f, 0.0f},
            {  7.0f, -5.0f, 0.0f},
            {  7.0f,  5.0f, 0.0f},
            { 12.0f,  0.0f, 0.0f}
        },

        // Goal positions
        {
            {0.0f,  12.0f, 0.0f},
            {0.0f, -12.0f, 0.0f}
        },

        // Goal required counts
        {
            2,
            2
        },

        // Goal movement configs
        {
            GoalMovementConfig(false, MoveDirection::Horizontal, 0.0f, 0.0f),
            GoalMovementConfig(false, MoveDirection::Horizontal, 0.0f, 0.0f)
        }
    },

    {
        3,
        "1-3",

        // Ball positions
        {
            {-30.0f, 10.0f, 0.0f},
            {-30.0f,  0.0f, 0.0f},
            {-10.0f, 10.0f, 0.0f},
            {-10.0f,  0.0f, 0.0f},
            { 10.0f, 10.0f, 0.0f},
            { 10.0f,  0.0f, 0.0f},
            { 30.0f, 10.0f, 0.0f},
            { 30.0f,  0.0f, 0.0f}
        },

        // Goal positions
        {
            {0.0f, -10.0f, 0.0f}
        },

        // Goal required counts
        {
            4
        },

        // Goal movement configs
        {
            GoalMovementConfig(true, MoveDirection::Horizontal, 30.0f, 0.7f)
        }
    },

    {
        4,
        "1-4",

        // Ball positions
        {
            { 20.0f, 0.0f, 0.0f},
            { 10.0f, 0.0f, 0.0f},
            {  0.0f, 1.5f, 0.0f},
            {-10.0f, 0.0f, 0.0f}
        },

        // Goal positions
        {
            {-20.0f, 0.0f, 0.0f}
        },

        // Goal required counts
        {
            1
        },

        // Goal movement configs
        {
            GoalMovementConfig(false, MoveDirection::Horizontal, 0.0f, 0.0f)
        }
    },

    {
        5,
        "1-5",

        // Ball positions
        {
            // 第一行（上）
            {  0.0f,  12.0f, 0.0f},

            // 第二行（中）
            { -5.0f,   7.0f, 0.0f},
            {  5.0f,   7.0f, 0.0f},
            {-10.0f,   6.4f, 0.0f},
            { 10.0f,   6.4f, 0.0f},

            // 第三行（下）
            { -5.5f,  -5.0f, 0.0f},
            {  5.5f,  -5.0f, 0.0f},
            {-13.0f, -10.0f, 0.0f},
            { 13.0f, -10.0f, 0.0f}
        },

        // Goal positions
        {
            {0.0f, 0.0f, 0.0f}
        },

        // Goal required counts
        {
            4
        },

        // Goal movement configs
        {
            GoalMovementConfig(false, MoveDirection::Horizontal, 0.0f, 0.0f)
        }
    },

    {
        6,
        "1-6",

        // Ball positions
        {
            // 第一行（上）
            {  0.0f,  12.0f, 0.0f},

            // 第二行（中）
            { -5.0f,   7.0f, 0.0f},
            {  5.0f,   7.0f, 0.0f},
            {-10.0f,   6.4f, 0.0f},
            { 10.0f,   6.4f, 0.0f},

            // 第三行（下）
            { -5.5f,  -5.0f, 0.0f},
            {  5.5f,  -5.0f, 0.0f},
            {-13.0f, -10.0f, 0.0f},
            { 13.0f, -10.0f, 0.0f}
        },

        // Goal positions
        {
            {0.0f, 0.0f, 0.0f}
        },

        // Goal required counts
        {
            4
        },

        // Goal movement configs
        {
            GoalMovementConfig(false, MoveDirection::Horizontal, 0.0f, 0.0f)
        }
    }
    };

    return configs;
}

void LevelManager::CreateLevels() {
    CleanupLevels();
    levelNames_.clear();

    std::vector<LevelConfig> configs = CreateLevelConfigs();

    for (const LevelConfig& config : configs) {
        GameScene* level = new GameScene();

        level->SetLevelConfig(
            config.levelNumber,
            config.ballPositions,
            config.goalPositions,
            config.goalRequiredCounts,
            config.goalMovementConfigs
        );

        levels_.push_back(level);
        levelNames_.push_back(config.levelName);
    }
}

void LevelManager::SetCurrentLevel(int level) {
    if (level >= 1 && level <= static_cast<int>(levels_.size())) {
        currentLevelIndex_ = level - 1;
    }
}

void LevelManager::Initialize() {
    if (!levels_.empty()) {
        levels_[currentLevelIndex_]->Initialize();

        isSceneEnd_ = false;
        shouldReturnToTitle_ = false;
        currentState_ = LevelState::Playing;
    }
}

void LevelManager::Update() {
    if (isSceneEnd_) {
        return;
    }

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

    case LevelState::GameComplete:
        UpdateGameCompleteState();
        break;
    }
}

void LevelManager::UpdatePlayingState() {
    if (currentLevelIndex_ >= levels_.size()) {
        return;
    }

    GameScene* currentLevel = levels_[currentLevelIndex_];
    if (!currentLevel) {
        return;
    }

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
    loadingScene_->Update();

    if (loadingScene_->IsSceneEnd()) {
        // 加载完成，进入下一关
        currentLevelIndex_++;

        if (currentLevelIndex_ < levels_.size()) {
            levels_[currentLevelIndex_]->Initialize();
            currentState_ = LevelState::Playing;
        }
        else {
            // 所有关卡完成
            currentState_ = LevelState::GameComplete;
            loadingScene_->StartLoading();
        }
    }
}

void LevelManager::UpdateTransitionState() {
    // 可以在这里添加其他过渡效果
    // 暂时为空
}

void LevelManager::UpdateGameCompleteState() {
    loadingScene_->Update();

    if (loadingScene_->IsSceneEnd()) {
        // 游戏完成Loading结束，设置场景结束
        isSceneEnd_ = true;
    }
}

void LevelManager::Draw() {
    switch (currentState_) {
    case LevelState::Playing:
        if (currentLevelIndex_ < levels_.size() && levels_[currentLevelIndex_]) {
            levels_[currentLevelIndex_]->Draw();
        }
        break;

    case LevelState::Loading:
    case LevelState::GameComplete:
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
        currentState_ = LevelState::GameComplete;
        loadingScene_->StartLoading();
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

void LevelManager::CleanupLevels() {
    for (GameScene* level : levels_) {
        if (level) {
            delete level;
        }
    }

    levels_.clear();
}