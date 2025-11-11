#pragma once
#include "../play/Ball.h"
#include "../play/Goal.h"
#include "../scene/stage.h"
#include "KamataEngine.h"
#include "../play/GameUI.h"
#include "../play/GameLogicManager.h" 
#include <vector> 
#include "SceneState.h"


using namespace KamataEngine;
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// 添加游戏结束状态获取方法
	bool IsGameOver() const { return gameState_ == GameState::GameOver; }
	bool IsSceneEnd() const { return isSceneEnd_; }

	// 获取下一个场景状态
	SceneState GetNextSceneState() const {
		return returnToTitle_ ? TITLE : RESULT;
	}

	// 新增关卡配置方法
	void SetLevelConfig(int levelNumber,
		const std::vector<KamataEngine::Vector3>& ballPositions,
		const KamataEngine::Vector3& goalPosition);

	int GetLevelNumber() const { return levelNumber_; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Stage* stage_ = nullptr;
	GameUI* gameUI_ = nullptr;
	GameLogicManager gameLogicManager_;

	std::vector<Ball*> balls_; // 改为存储多个 Ball 的向量
	std::vector<Goal*> goals_; 

	
	Camera camera_;

	// 游戏状态枚举
	enum class GameState {
		Tutorial,  // 教程图片播放
		StartWait, // 等待开始
		StartAnim, // 开始动画（缩小动画）
		Playing,   // 游戏进行中
		GameOver   // 游戏结束
	};
	GameState gameState_ = GameState::Tutorial;

    std::vector<uint32_t> tutorialTextureHandles_; // 教程图片纹理句柄
	std::vector<KamataEngine::Sprite*> tutorialSprites_; 
	 uint32_t startTextureHandle_ = 0;             // 开始图片纹理句柄
	KamataEngine::Sprite* startSprite_ = nullptr; // 开始图片精灵
	int currentTutorialIndex_ = 0;           // 当前教程图片索引
	float startTimer_ = 0.0f;                // 开始计时器
	float animTimer_ = 0.0f;                 // 动画计时器
	bool showStart_ = false;                 // 是否显示开始图片
	Vector2 startSize_ = {3840.0f, 2160.0f}; // 开始图片的初始尺寸（3倍：1280*3, 720*3）
	Vector2 targetSize_ = {1280.0f, 720.0f}; // 开始图片的目标尺寸
	const float animDuration_ = 1.0f;        // 动画持续时间（秒）
	const float displayDuration_ = 1.0f;     // 显示持续时间（秒）
	

	// 教程系统方法
	void LoadTutorialTextures();
	void UpdateTutorial();
	void UpdateStartWait();
	void UpdateStartAnim();
	void DrawTutorial();
	void StartGame();
	void RestartLevel();
	void ReturnToTitle();
	void GameOver();


	
	bool isSceneEnd_ = false;  // 场景是否结束
	bool returnToTitle_ = false;

	// 新增关卡相关变量
	int levelNumber_ = 1;
	std::vector<KamataEngine::Vector3> levelBallPositions_;
	KamataEngine::Vector3 levelGoalPosition_ = { 25.0f, 0.0f, 0.0f };

	// 新增初始化方法
	void InitializeLevelObjects();

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};