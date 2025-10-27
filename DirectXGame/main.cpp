#include "KamataEngine.h"
#include "scene/GameScene.h"
#include "scene/ResultScene.h"  
#include <Windows.h>
#include "scene/TitleScene.h"
#include "scene/LoadingScene.h"
#include "scene/SceneState.h"
using namespace KamataEngine;



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	KamataEngine::Initialize(L"LE3C_17_トウ_カグン");
	GameScene* gameScene = nullptr;
	TitleScene* titleScene = nullptr;
	LoadingScene* loadingScene = nullptr;
	ResultScene* resultScene = nullptr;

	SceneState currentSceneState = TITLE;
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
			
				delete titleScene;
				titleScene = nullptr;

				loadingScene->StartLoading();
				currentSceneState = LOADING;
				
			}
			break;
		case LOADING:
			loadingScene->Updata();
			if (loadingScene->isLoadingComplete()) {
				gameScene = new GameScene();
				gameScene->Initialize();

				currentSceneState = GAME;
			}
			break;
		case GAME:
			if (gameScene) {
				gameScene->Update();
				if (gameScene->IsSceneEnd()) {
					delete gameScene;
					gameScene = nullptr;

					resultScene = new ResultScene();
					resultScene->Initialize();
					currentSceneState = RESULT;
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

		case LOADING:
			loadingScene->Draw();
			break;

		case GAME:
			if (gameScene) {
				gameScene->Draw();
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

	delete gameScene;
	gameScene = nullptr;
	delete titleScene;
	titleScene = nullptr;
	delete loadingScene;
	loadingScene = nullptr;

	KamataEngine::Finalize();

	return 0;
}