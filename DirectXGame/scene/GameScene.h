#pragma once
#include "../Ball.h"
#include "../Goal.h"
#include "../stage.h"
#include "KamataEngine.h"
#include <vector> 


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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Stage* stage_ = nullptr;
	std::vector<Ball*> balls_; // 改为存储多个 Ball 的向量
	std::vector<Goal*> goals_; 
	Vector2 mousePos = {0, 0};
    bool IsMouseOverBall(Ball* ball, const Vector2& mousePos);
	KamataEngine::Vector3 WorldToScreen(const KamataEngine::Vector3& worldPos);
	Camera camera_;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};