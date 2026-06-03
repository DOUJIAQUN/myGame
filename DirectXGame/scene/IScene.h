// IScene.h
#pragma once
#include "SceneState.h"

class IScene {
public:
    virtual ~IScene() = default;          // 虚析构函数

    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual bool IsSceneEnd() const = 0;   // 场景是否结束
    virtual SceneState GetNextSceneState() const = 0; // 结束后的下一个场景
};