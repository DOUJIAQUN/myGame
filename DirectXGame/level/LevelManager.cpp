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
    }
}
std::string LevelManager::GetCurrentLevelName() const {
    if (currentLevelIndex_ >= levels_.size()) return "Unknown";

    int levelNumber = levels_[currentLevelIndex_]->GetLevelNumber();
    switch (levelNumber) {
    case 1: return "1-1";
    case 2: return "1-2";
    case 3: return "1-3";
    case 4: return "1-4";
    default: return "Unknown";
    }
}


void LevelManager::CreateLevels() {
  
    // 第1-1关配置
    GameScene* level1_1 = new GameScene();
   
    std::vector<KamataEngine::Vector3> level1BallPositions = {
        {-30.0f, 0.0f, 0.0f},
        {-20.0f, 0.0f, 0.0f},
        { 0.0f, 0.0f, 0.0f},
        {20.0f, 0.0f, 0.0f}
    };
    std::vector<KamataEngine::Vector3> level1GoalPositions = {
       {30.0f, 0.0f, 0.0f}  // 单个终点
    };
    std::vector<int> level1GoalRequiredCounts = { 1 };
    // 第1-1关终点不移动
    std::vector<GoalMovementConfig> level1MovementConfigs = {
        GoalMovementConfig(false, MoveDirection::Horizontal, 0.0f, 0.0f)
    };

    level1_1->SetLevelConfig(1, level1BallPositions, level1GoalPositions);
    levels_.push_back(level1_1);
  

    // 第1-2关配置
    GameScene* level1_2 = new GameScene();
   
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

    // 第1-2关终点也不移动
    std::vector<GoalMovementConfig> level2MovementConfigs = {
        GoalMovementConfig(false, MoveDirection::Horizontal, 0.0f, 0.0f),
        GoalMovementConfig(false, MoveDirection::Horizontal, 0.0f, 0.0f)
    };
    level1_2->SetLevelConfig(2, level2BallPositions, level2GoalPositions,level2GoalRequiredCounts);
    levels_.push_back(level1_2);
  

    // 第1-3关配置
    GameScene* level1_3 = new GameScene();
    std::vector<KamataEngine::Vector3> level1_3BallPositions = {
        {-30.0f, 10.0f, 0.0f},
        {-30.0f, 0.0f, 0.0f},
        {-10.0f, 10.0f, 0.0f},
        {-10.0f, 0.0f, 0.0f},
        {10.0f, 10.0f, 0.0f },
        {10.0f, 0.0f, 0.0f },
        {30.0f, 10.0f, 0.0f },
        {30.0f, 0.0f, 0.0f }
    };
    std::vector<KamataEngine::Vector3> level1_3GoalPositions = {
        {0.0f, -10.0f, 0.0f}  
    };
    std::vector<int> level1_3GoalRequiredCounts = {
        4  
    };

    // 第1-3关终点水平移动
    std::vector<GoalMovementConfig> level1_3MovementConfigs = {
        GoalMovementConfig(true, MoveDirection::Horizontal, 30.0f, 0.7f)
    };

    level1_3->SetLevelConfig(3, level1_3BallPositions, level1_3GoalPositions, level1_3GoalRequiredCounts);
    
        // 确保调用的是正确的SetLevelConfig重载
    level1_3->SetLevelConfig(3, 
        level1_3BallPositions, 
        level1_3GoalPositions, 
        level1_3GoalRequiredCounts, 
        level1_3MovementConfigs);  // 确保传递了移动配置
    levels_.push_back(level1_3);

    // 第1-4关配置
    GameScene* level1_4 = new GameScene();
    std::vector<KamataEngine::Vector3> level1_4BallPositions = {
        {20.0f, 0.0f, 0.0f},
        {10.0f, 0.0f, 0.0f},
        {0.0f, 1.5f, 0.0f},
        {-10.0f, 0.0f, 0.0f}
    };
    std::vector<KamataEngine::Vector3> level1_4GoalPositions = {
        {-20.0f, 0.0f, 0.0f}
    };
    std::vector<int> level1_4GoalRequiredCounts = { 1 };
    std::vector<GoalMovementConfig> level1_4MovementConfigs = {
        GoalMovementConfig(false, MoveDirection::Horizontal, 0.0f, 0.0f)
    };
    level1_4->SetLevelConfig(4,
        level1_4BallPositions,
        level1_4GoalPositions,
        level1_4GoalRequiredCounts,
        level1_4MovementConfigs);
    levels_.push_back(level1_4);


// 第1-5关配置
    GameScene* level1_5 = new GameScene();
    std::vector<KamataEngine::Vector3> level1_5BallPositions = {
        // 第一行（上）
        {0.0f, 12.0f, 0.0f},
        // 第二行（中）
        {-5.0f, 7.0f, 0.0f},
        {5.0f, 7.0f, 0.0f},
        {-10.0f, 6.4f, 0.0f},
        {10.0f, 6.4f, 0.0f},
        // 第三行（下）
         {-5.5f, -5.0f, 0.0f},
        {5.5f, -5.0f, 0.0f},
        {-13.0f, -10.0f, 0.0f},
        {13.0f, -10.0f, 0.0f}
    };
    std::vector<KamataEngine::Vector3> level1_5GoalPositions = {
        {0.0f, 0.0f, 0.0f}
    };
    std::vector<int> level1_5GoalRequiredCounts = {
        4  // 需要进入4次
    };
    // 第1-5关终点不移动
    std::vector<GoalMovementConfig> level1_5MovementConfigs = {
        GoalMovementConfig(false, MoveDirection::Horizontal, 0.0f, 0.0f)
    };

    level1_5->SetLevelConfig(5,
        level1_5BallPositions,
        level1_5GoalPositions,
        level1_5GoalRequiredCounts,
        level1_5MovementConfigs);
    levels_.push_back(level1_5);


    // 第1-5关配置
    GameScene* level1_6 = new GameScene();
    std::vector<KamataEngine::Vector3> level1_6BallPositions = {
        // 第一行（上）
        {0.0f, 12.0f, 0.0f},
        // 第二行（中）
        {-5.0f, 7.0f, 0.0f},
        {5.0f, 7.0f, 0.0f},
        {-10.0f, 6.4f, 0.0f},
        {10.0f, 6.4f, 0.0f},
        // 第三行（下）
         {-5.5f, -5.0f, 0.0f},
        {5.5f, -5.0f, 0.0f},
        {-13.0f, -10.0f, 0.0f},
        {13.0f, -10.0f, 0.0f}
    };
    std::vector<KamataEngine::Vector3> level1_6GoalPositions = {
        {0.0f, 0.0f, 0.0f}
    };
    std::vector<int> level1_6GoalRequiredCounts = {
        4  // 需要进入4次
    };
    // 第1-5关终点不移动
    std::vector<GoalMovementConfig> level1_6MovementConfigs = {
        GoalMovementConfig(false, MoveDirection::Horizontal, 0.0f, 0.0f)
    };

    level1_6->SetLevelConfig(6,
        level1_6BallPositions,
        level1_6GoalPositions,
        level1_6GoalRequiredCounts,
        level1_6MovementConfigs);
    levels_.push_back(level1_6);

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

    case LevelState::GameComplete:  // 新增：游戏完成状态
        UpdateGameCompleteState();
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
    loadingScene_->Updata();
    if (loadingScene_->isLoadingComplete()) {
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
        loadingScene_->StartLoading();  //这里也要重置Loading场景
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
    for (auto level : levels_) {
        if (level) {
            delete level;
        }
    }
    levels_.clear();
}