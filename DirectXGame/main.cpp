#include "KamataEngine.h"
#include "level/LevelManager.h"
#include "scene/ResultScene.h"
#include <Windows.h>
#include "scene/TitleScene.h"
#include "scene/LoadingScene.h"
#include "scene/StageSelectScene.h"
#include "scene/SceneState.h"
#include "scene/IScene.h"
#include "DebugLogger.h"

#include <memory>

using namespace KamataEngine;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
    KamataEngine::Initialize(L"LE4C_08_トウ_カグン");
    DirectXCommon* dxCommon = DirectXCommon::GetInstance();

    std::unique_ptr<IScene> currentScene = nullptr;
    SceneState currentState = TITLE;
    int selectedLevel = 1;  // 用于 LevelManager 初始化

    // 场景循环
    while (true) {
        if (KamataEngine::Update()) {
            break;
        }

        // 场景切换：如果当前场景为空或已结束，则创建下一个场景
        if (currentScene == nullptr || currentScene->IsSceneEnd()) {
            SceneState nextState = (currentScene == nullptr) ? TITLE : currentScene->GetNextSceneState();
            
            currentScene = nullptr;

            switch (nextState) {
            case TITLE:
                currentScene = std::make_unique<TitleScene>();
                break;
            case STAGE_SELECT:
                currentScene = std::make_unique<StageSelectScene>();
                break;
            case LOADING:
                currentScene = std::make_unique<LoadingScene>();
                // LoadingScene 开始加载
                static_cast<LoadingScene*>(currentScene.get())->StartLoading();
                break;
            case GAME:
                currentScene = std::make_unique<LevelManager>();
                static_cast<LevelManager*>(currentScene.get())->SetCurrentLevel(selectedLevel);
                break;
            case RESULT:
                currentScene = std::make_unique<ResultScene>();
                break;
            }

            if (currentScene) {
                currentScene->Initialize();
                // 如果是从 Title 或 StageSelect 获取了 selectedLevel，需要传递给 LevelManager
                // 但 LevelManager 在 GAME 状态才创建，所以上面已经处理
                // 对于从其他状态得到 selectedLevel 的情况，例如 Title 结束时要记录
                if (nextState == LOADING && (currentState == TITLE || currentState == STAGE_SELECT)) {
                    // selectedLevel 已经在上层状态中更新，这里无需额外操作
                }
            }
        }

        // 更新当前场景
        if (currentScene) {
            currentScene->Update();
        }

        // 描画開始
        dxCommon->PreDraw();

        if (currentScene) {
            currentScene->Draw();
        }

        // 描画終了
        dxCommon->PostDraw();

        // 记录当前状态用于下次循环（实际上通过 nextState 已经确定）
        // 但需要更新 selectedLevel 信息（从 TitleScene 或 StageSelectScene 获取）
        if (currentScene && currentScene->IsSceneEnd()) {
            SceneState endingState = currentScene->GetNextSceneState();
            if (endingState == LOADING) {
                // 尝试从当前场景获取 selectedLevel
                if (dynamic_cast<TitleScene*>(currentScene.get())) {
                    selectedLevel = static_cast<TitleScene*>(currentScene.get())->GetSelectedLevel();
                }
                else if (dynamic_cast<StageSelectScene*>(currentScene.get())) {
                    selectedLevel = static_cast<StageSelectScene*>(currentScene.get())->GetSelectedLevel();
                }
            }
        }
    }

   
    KamataEngine::Finalize();
    return 0;
}