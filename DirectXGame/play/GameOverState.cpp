#include "GameOverState.h"
#include "../scene/GameScene.h"

void GameOverState::Enter(GameScene* scene) {
    // 标记场景结束，将在下一帧让外部管理器切换场景
    scene->SetSceneEndFlag();
}

void GameOverState::Update(GameScene* scene) {
    // 通常不需要额外更新，等待外部系统切换
}

void GameOverState::Exit(GameScene* scene) {
    // 清理工作（如果需要）
}