#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete stage_;
	for (Ball* ball : balls_) {
		delete ball;
	}
	balls_.clear(); // 清空向量
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	camera_.Initialize();

	stage_ = new Stage();
	stage_->Initialize();

	  // 创建两个 Ball 实例并设置不同位置
    const int ballCount = 4;
	KamataEngine::Vector3 positions[4] = {
	    {-30.0f, 0.0f, 0.0f}, // 第一个小球
	    {-25.0f, 0.0f, 0.0f}, // 第二个小球
	    {-5.0f, 0.0f, 0.0f}, // 第三个小球
	    {15.0f, 0.0f, 0.0f}  // 第四个小球
	};
	for (int i = 0; i < ballCount; i++) {
		Ball* ball = new Ball();
		ball->Initialize(&camera_);
		ball->SetPosition(positions[i]);
		balls_.push_back(ball);
	}
	const int GoalCount = 1;
	KamataEngine::Vector3 goalPositions[1] = {
	    {25.0f, 0.0f, 0.0f},
	    
	};
	for (int i = 0; i < GoalCount; i++) {
		Goal* goal = new Goal();
		goal->Initialize(&camera_);
		goal->SetPosition(goalPositions[i]);
		goals_.push_back(goal);
	}
}



void GameScene::Update() {
	stage_->Update();

		// 获取鼠标位置
	mousePos = Input::GetInstance()->GetMousePosition();

	  
	if (input_->IsTriggerMouse(0)) {
		// 遍历所有球体，检查鼠标是否点击到球体
		for (size_t i = 0; i < balls_.size(); i++) {
			Ball* clickedBall = balls_[i];

			// 检查球体是否活跃且未被爆炸
			if (clickedBall->IsActive() && !clickedBall->IsExploded()) {
				// 使用IsMouseOverBall检测鼠标是否在球体范围内
				if (IsMouseOverBall(clickedBall, mousePos)) {
					// 这里处理鼠标点击到小球的情况
					clickedBall->Explode();

					// 获取爆炸位置
					KamataEngine::Vector3 explosionPos = clickedBall->GetPosition();
					const float explosionRadius = 11.0f;
					const float explosionForce = 1.0f;

					// 对范围内的其他球体施加爆炸力
					for (size_t j = 0; j < balls_.size(); j++) {
						if (i == j)
							continue; // 跳过被点击的球体本身

						Ball* otherBall = balls_[j];
						if (otherBall->IsActive()) {
							float distance = myMath::Distance(explosionPos, otherBall->GetPosition());

							if (distance <= explosionRadius) {
								// 计算爆炸方向（从爆炸中心指向球体）
								KamataEngine::Vector3 direction = myMath::Subtract(otherBall->GetPosition(), explosionPos);
								direction = myMath::Normalize(direction);

								
								
								KamataEngine::Vector3 force = myMath::Multiply(explosionForce, direction);

								// 应用爆炸力
								otherBall->ApplyExplosionForce(force);
							}
						}
					}
					break; // 只处理第一个被点击的球体
				}
			}
		}
	}
	// 更新所有 Ball
	for (Ball* ball : balls_) {
		ball->Update();
	}
    
	camera_.UpdateMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	stage_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (Ball* ball : balls_) {
		ball->Draw();
	}
	for (Goal* goal : goals_) {
		goal->Draw();
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

bool GameScene::IsMouseOverBall(Ball* ball, const Vector2& mousePos) {
	// 获取球体的世界坐标
	KamataEngine::Vector3 worldPos = ball->GetPosition();

	// 将世界坐标转换为屏幕坐标
	KamataEngine::Vector3 screenPos = WorldToScreen(worldPos);

	// 球体在屏幕上的半径（根据球体大小和距离调整）
	// 球体实际半径是2（从worldTransform_.scale_ = {2, 2, 2}推断）
	// 可视范围调整后的屏幕半径
	float ballRadius = 20.0f; // 这个值可能需要根据实际显示效果调整

	// 计算鼠标位置与球体屏幕位置的距离
	float distance = std::sqrt(std::pow(mousePos.x - screenPos.x, 2) + std::pow(mousePos.y - screenPos.y, 2));

	// 圆形碰撞检测：检查鼠标是否在圆形范围内
	return distance <= ballRadius;
}


KamataEngine::Vector3 GameScene::WorldToScreen(const KamataEngine::Vector3& worldPos) {
	// 确保相机矩阵已经更新
	camera_.UpdateMatrix();

	// 使用相机的视图投影矩阵进行准确的坐标转换
	const KamataEngine::Matrix4x4& viewMatrix = camera_.matView;
	const KamataEngine::Matrix4x4& projectionMatrix = camera_.matProjection;
	KamataEngine::Matrix4x4 viewProjectionMatrix = myMath::Multiply(viewMatrix, projectionMatrix);

	// 变换到齐次裁剪空间
	KamataEngine::Vector4 worldPos4 = {worldPos.x, worldPos.y, worldPos.z, 1.0f};

	// 手动进行矩阵变换（因为myMath::Transform可能不支持Vector4）
	KamataEngine::Vector4 clipPos;
	clipPos.x = worldPos4.x * viewProjectionMatrix.m[0][0] + worldPos4.y * viewProjectionMatrix.m[1][0] + worldPos4.z * viewProjectionMatrix.m[2][0] + worldPos4.w * viewProjectionMatrix.m[3][0];
	clipPos.y = worldPos4.x * viewProjectionMatrix.m[0][1] + worldPos4.y * viewProjectionMatrix.m[1][1] + worldPos4.z * viewProjectionMatrix.m[2][1] + worldPos4.w * viewProjectionMatrix.m[3][1];
	clipPos.z = worldPos4.x * viewProjectionMatrix.m[0][2] + worldPos4.y * viewProjectionMatrix.m[1][2] + worldPos4.z * viewProjectionMatrix.m[2][2] + worldPos4.w * viewProjectionMatrix.m[3][2];
	clipPos.w = worldPos4.x * viewProjectionMatrix.m[0][3] + worldPos4.y * viewProjectionMatrix.m[1][3] + worldPos4.z * viewProjectionMatrix.m[2][3] + worldPos4.w * viewProjectionMatrix.m[3][3];

	// 透视除法
	if (clipPos.w != 0.0f) {
		clipPos.x /= clipPos.w;
		clipPos.y /= clipPos.w;
		clipPos.z /= clipPos.w;
	}

	// 转换到屏幕坐标
	// 假设屏幕分辨率为1280x720
	float screenX = (clipPos.x + 1.0f) * 0.5f * 1280.0f;
	float screenY = (1.0f - (clipPos.y + 1.0f) * 0.5f) * 720.0f;

	return {screenX, screenY, clipPos.z};
}