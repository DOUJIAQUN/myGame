// IScene.h
#pragma once
#include "SceneState.h"

namespace MyEngine {


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
/// シーン管理側へ、このシーンが終了したかどうかを返す。
/// </summary>
    virtual bool IsSceneEnd() const = 0;   // 场景是否结束
    /// <summary>
/// シーン終了後に遷移する次の SceneState を返す。
/// </summary>
    virtual SceneState GetNextSceneState() const = 0; // 结束后的下一个场景
};

} // namespace MyEngine
