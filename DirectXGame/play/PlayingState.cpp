#include "PlayingState.h"
#include "../scene/GameScene.h"
#include "GameOverState.h"   // 通关后切换到 GameOverState
#include <memory>

void PlayingState::Enter(GameScene* scene) {
    // 可选：重置游戏相关的标志
    // 例如 scene->ResetLevelCompletion();
}

void PlayingState::Update(GameScene* scene) {
    // 调用 GameScene 中各个子系统更新
    scene->UpdateStageInternal();
    scene->UpdateUIInternal();
    scene->UpdateGameLogicInternal();
    scene->UpdateBallsAndGoalsInternal();

    // 检查是否通关
    if (scene->IsLevelComplete()) {
        scene->ChangeState(std::make_unique<GameOverState>());
    }
}

void PlayingState::Exit(GameScene* scene) {
    // 可选：播放通关音效等
}