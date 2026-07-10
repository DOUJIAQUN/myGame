#pragma once
#include "IGameState.h"

/// <summary>
/// ゲームクリア後、リザルトへ遷移するまでの状態を管理するクラス。
/// </summary>
class GameOverState : public IGameState {
public:
    /// <summary>
    /// 状態に入った時の初期化処理を行う。
    /// </summary>
    void Enter(GameScene* scene) override;
    /// <summary>
    /// 毎フレームの更新処理を行う。
    /// </summary>
    void Update(GameScene* scene) override;
    /// <summary>
    /// 状態を抜ける時の終了処理を行う。
    /// </summary>
    void Exit(GameScene* scene) override;
};
