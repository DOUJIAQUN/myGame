#pragma once
#include "../Ball.h"
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
	Camera camera_;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};