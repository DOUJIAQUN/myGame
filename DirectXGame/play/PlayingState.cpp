#include "PlayingState.h"
#include "../scene/GameScene.h"
#include "GameOverState.h"   // 通关后切换到 GameOverState
#include <memory>

// 関数コメント: Enter の処理を実行する。
void PlayingState::Enter(GameScene* scene) {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Enter の役割を実現する。
    // 可选：重置游戏相关的标志
    // 例如 scene->ResetLevelCompletion();
}

// 関数コメント: Update の処理を実行する。
void PlayingState::Update(GameScene* scene) {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Update の役割を実現する。
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

// 関数コメント: Exit の処理を実行する。
void PlayingState::Exit(GameScene* scene) {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Exit の役割を実現する。
    // 可选：播放通关音效等
}
