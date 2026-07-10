// IScene.h
#pragma once
#include "SceneState.h"

/// <summary>
/// 各シーンに共通する初期化、更新、描画、遷移判定を定義するインターフェース。
/// </summary>
class IScene {
public:
    virtual ~IScene() = default;          // 虚析构函数

    /// <summary>
    /// オブジェクトやシーンの初期化処理を行う。
    /// </summary>
    virtual void Initialize() = 0;
    /// <summary>
    /// 毎フレームの更新処理を行う。
    /// </summary>
    virtual void Update() = 0;
    /// <summary>
    /// 描画処理を行う。
    /// </summary>
    virtual void Draw() = 0;
    /// <summary>
    /// IsSceneEnd に関する処理を行う。
    /// </summary>
    virtual bool IsSceneEnd() const = 0;   // 场景是否结束
    /// <summary>
    /// GetNextSceneState に関する処理を行う。
    /// </summary>
    virtual SceneState GetNextSceneState() const = 0; // 结束后的下一个场景
};
