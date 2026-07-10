#pragma once

class GameScene;

/// <summary>
/// GameScene の状態を切り替えるための State インターフェース。
/// </summary>
class IGameState {
public:
    virtual ~IGameState() = default;
    /// <summary>
    /// 状態に入った時の初期化処理を行う。
    /// </summary>
    virtual void Enter(GameScene* scene) = 0;   // 进入状态时的初始化
    /// <summary>
    /// 毎フレームの更新処理を行う。
    /// </summary>
    virtual void Update(GameScene* scene) = 0;  // 每帧更新
    /// <summary>
    /// 状態を抜ける時の終了処理を行う。
    /// </summary>
    virtual void Exit(GameScene* scene) = 0;    // 离开状态时的清理
};
