#include "GameOverState.h"
#include "../scene/GameScene.h"

// 関数コメント: Enter の処理を実行する。
void GameOverState::Enter(GameScene* scene) {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Enter の役割を実現する。
    // 标记场景结束，将在下一帧让外部管理器切换场景
    scene->SetSceneEndFlag();
}

// 関数コメント: Update の処理を実行する。
void GameOverState::Update(GameScene* scene) {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Update の役割を実現する。
    // 通常不需要额外更新，等待外部系统切换
}

// 関数コメント: Exit の処理を実行する。
void GameOverState::Exit(GameScene* scene) {
    // 処理コメント: 必要な状態確認やデータ更新を行い、Exit の役割を実現する。
    // 清理工作（如果需要）
}
