#include "KamataEngine.h"
#include "level/LevelManager.h"  
#include "scene/ResultScene.h"  
#include <Windows.h>
#include "scene/TitleScene.h"
#include "scene/LoadingScene.h"
#include "scene/StageSelectScene.h"
#include "scene/SceneState.h"
#include "DebugLogger.h"

using namespace KamataEngine;



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	KamataEngine::Initialize(L"LE3C_17_トウ_カグン");
	LevelManager* levelManager = nullptr;
	TitleScene* titleScene = nullptr;
	LoadingScene* loadingScene = nullptr;
	ResultScene* resultScene = nullptr;
	StageSelectScene* stageSelectScene = nullptr;

	SceneState currentSceneState = TITLE;
	// 保存关卡选择信息
	int selectedLevel = 1;// 默认第一关

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	titleScene = new TitleScene();
	titleScene->Initialize();

	loadingScene = new LoadingScene();
	loadingScene->Initialize();

	while (true) {
		if (KamataEngine::Update()) {
			break;
		}
		
		switch (currentSceneState) { 
		case TITLE:
			if (titleScene&&!titleScene->IsSceneEnd()) {
				titleScene->Update();
			} 
			else {
				selectedLevel = titleScene->GetSelectedLevel();
				delete titleScene;
				titleScene = nullptr;

				if (selectedLevel == 0) {
					// 进入关卡选择界面
					stageSelectScene = new StageSelectScene();
					stageSelectScene->Initialize();
					currentSceneState = STAGE_SELECT;
				}
				else {
					// 直接开始第一关
					loadingScene->StartLoading();
					currentSceneState = LOADING;
				}
				
			}
			break;
		case STAGE_SELECT:  // 新增：关卡选择状态
			if (stageSelectScene && !stageSelectScene->IsSceneEnd()) {
				stageSelectScene->Update();
			}
			else {
				selectedLevel = stageSelectScene->GetSelectedLevel();
				delete stageSelectScene;
				stageSelectScene = nullptr;

				if (selectedLevel == 0) {
					// 返回标题
					titleScene = new TitleScene();
					titleScene->Initialize();
					currentSceneState = TITLE;
				}
				else {
					// 开始选定的关卡
					loadingScene->StartLoading();
					currentSceneState = LOADING;
					
				}
			}
			break;
		case LOADING:
			loadingScene->Updata();
			if (loadingScene->isLoadingComplete()) 
			 {
				levelManager = new LevelManager();  
				
	
				// 新增：如果从关卡选择界面选择了特定关卡，设置当前关卡
				levelManager->SetCurrentLevel(selectedLevel);
			
				levelManager->Initialize();
				currentSceneState = GAME;
			}
			break;
		case GAME:
			if (levelManager && !levelManager->IsSceneEnd()) {
				levelManager->Update();
				if (levelManager->IsSceneEnd()) {
					SceneState nextState = levelManager->GetNextSceneState();

					delete levelManager;
					levelManager = nullptr;

					if (nextState == TITLE) {
						titleScene = new TitleScene();
						titleScene->Initialize();
						currentSceneState = TITLE;
					}
					else {
						resultScene = new ResultScene();
						resultScene->Initialize();
						currentSceneState = RESULT;
					}
				}
			}
			break;
		case RESULT:
			if (resultScene) {
				resultScene->Update();
				if (resultScene->IsSceneEnd()) {
					delete resultScene;
					resultScene = nullptr;

					// 重新开始游戏或返回标题
					// 这里简单处理为返回标题
					titleScene = new TitleScene();
					titleScene->Initialize();
					currentSceneState = TITLE;
				}
			}
			break;
		}




		// 描画開始
		dxCommon->PreDraw();


		switch (currentSceneState) {
		case TITLE:
			if (titleScene) {
				titleScene->Draw();
			}
			break;

		case STAGE_SELECT:  // 新增：关卡选择绘制
			if (stageSelectScene) {
				stageSelectScene->Draw();
			}
			break;

		case LOADING:
			loadingScene->Draw();
			break;

		case GAME:
			if (levelManager) {
				levelManager->Draw();
			}
			break;
		case RESULT:
			if (resultScene) {
				resultScene->Draw();
			}
			break;
		}


		// 描画終了
		dxCommon->PostDraw();
	}

	if (levelManager) delete levelManager;
	if (titleScene) delete titleScene;
	if (stageSelectScene) delete stageSelectScene;
	if (loadingScene) delete loadingScene;
	if (resultScene) delete resultScene;
	KamataEngine::Finalize();

	return 0;
}