#pragma once
#include "../play/Ball.h"
#include "../play/Goal.h"
#include "../scene/stage.h"
#include "KamataEngine.h"
#include "../play/GameUI.h"
#include "../play/GameLogicManager.h" 
#include "../play/GoalMovementConfig.h" 
#include <vector> 
#include "SceneState.h"
#include "IScene.h"
#include "../play/IGameState.h"


using namespace KamataEngine;
/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public IScene {

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
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;



	// 添加游戏结束状态获取方法
	
	bool IsSceneEnd() const override { return isSceneEnd_; }

	// 获取下一个场景状态
	SceneState GetNextSceneState() const override {
		return returnToTitle_ ? TITLE : RESULT;
	}

	// 关卡配置方法
	void SetLevelConfig(int levelNumber,
		const std::vector<KamataEngine::Vector3>& ballPositions,
		const std::vector<KamataEngine::Vector3>& goalPosition,
		const std::vector<int>& goalRequiredCounts = {},
		const std::vector<GoalMovementConfig>& goalMovementConfigs = {});

	int GetLevelNumber() const { return levelNumber_; }

	void ChangeState(IGameState* newState);

	// 供状态类调用的公共接口
	void UpdateStageInternal();          // 更新背景
	void UpdateUIInternal();             // 更新UI（检测按钮点击等）
	void UpdateGameLogicInternal();      // 更新碰撞检测等
	void UpdateBallsAndGoalsInternal();  // 更新所有球体和目标
	bool IsLevelComplete() const;        // 检查是否通关
	void SetSceneEndFlag();              // 设置 isSceneEnd_ = true

	// 供状态类调用的其他公共方法
	void RestartLevel();
	void ReturnToTitle();

	// 教程相关（供 UpdateTutorial 等使用）
	bool IsTutorialFinished() const { return currentTutorialIndex_ >= tutorialSprites_.size(); }
	void ResetTutorial() { currentTutorialIndex_ = 0; }

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
	enum class GameFlowState {
		Tutorial,  // 教程图片播放
		StartWait, // 等待开始
		StartAnim, // 开始动画（缩小动画）
	};
	GameFlowState gameFlowState_ = GameFlowState::Tutorial;

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
	const float displayDuration_ = 0.0f;     // 显示持续时间（秒）
	

	// 关卡数据
	int levelNumber_ = 1;
	std::vector<KamataEngine::Vector3> levelBallPositions_;
	std::vector<KamataEngine::Vector3> levelGoalPositions_;
	std::vector<int> levelGoalRequiredCounts_;
	std::vector<GoalMovementConfig> levelGoalMovementConfigs_;


	
	bool isSceneEnd_ = false;  // 场景是否结束
	bool returnToTitle_ = false;



	// 状态模式成员
	IGameState* currentState_ = nullptr;

	// 内部辅助函数
	void LoadTutorialTextures();
	void UpdateTutorial();
	void UpdateStartWait();
	void UpdateStartAnim();
	void StartGame();           // 从动画切换到 Playing 状态
	void DrawTutorial();

	void InitializeLevelObjects();
	void CheckLevelCompletion();
	void ResetLevelCompletion();
	void GameOver();           // 旧式 GameOver，现在内部调用 ChangeState
};